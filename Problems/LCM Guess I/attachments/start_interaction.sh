#!/usr/bin/env bash
# Initialize the variables
COUNTER=0
if [ -z "$TIME_LIMIT" ]; then
  TIME_LIMIT=3
fi

if [ -z "$TIME_RESOLUTION" ]; then
  TIME_RESOLUTION=0.05
fi

# Parse the arguments
for ARG in "$@"; do
  if [[ "$ARG" == "@" ]]; then 
	COUNTER=$((COUNTER+1))
	continue;
  fi
  case "$COUNTER" in
    0)
      if [ -z "$TEST_FILE" ]; then
        TEST_FILE="$ARG"
      fi
      ;;

    1)
      INTERACTOR=("${INTERACTOR[@]}" "$ARG")
      ;;

    2)
      PROGRAM=("${PROGRAM[@]}" "$ARG")
      ;;
    *)
      ;;
  esac
done

# Check if the arguments are valid
if [ "$COUNTER" -ne 2 ]; then
	echo "Usage: $0 TEST_CASE @ INTERACTOR [ARGS...] @ PROGRAM [ARGS...]"
	exit 1
else

  if [ -z "$TEST_FILE" ]; then
    echo "Warning: No testfile provided. Defaulting to STDIN. Use '-' to specify STDIN." >&2
    TEST_FILE="-"
  fi

  echo "Starting Interaction with the following parameters:"

  if [[ "$TEST_FILE" == "-" ]]; then
    echo "  Test Case: STDIN"
  else
    echo "  Test Case:" "${TEST_FILE}"
  fi
	echo "  Interactor:" "${INTERACTOR[@]}"
	echo "  Program File:" "${PROGRAM[@]}"
	echo "  Time Limit:" "${TIME_LIMIT} seconds"
fi

if [ -n "$NDEBUG" ]; then
	unset DEBUG
elif [ -z "$DEBUG" ]; then
	export DEBUG=interaction.txt
fi

if [ -f "$DEBUG" ]; then
	rm "$DEBUG"
fi

# Create the working directory and the pipes
PIPE_DIR=$(mktemp -d)
mkfifo "$PIPE_DIR"/0
mkfifo "$PIPE_DIR"/1

# If the test file is STDIN, then read the input from the user and write it to a temporary file
if [[ "$TEST_FILE" == "-" ]]; then
	IFS= read -r N
	IFS= read -r P
	INTERACTOR_TMP_FILE="$PIPE_DIR"/test.in
	echo "$N" >>"$INTERACTOR_TMP_FILE"
	echo "$P" >>"$INTERACTOR_TMP_FILE"
	TEST_FILE="$INTERACTOR_TMP_FILE"
fi

# Preparing Clean up
cleanUp()
{
  rm -rf -- "$PIPE_DIR"
  kill %% 2>/dev/null
}
# Trap the signals
trap cleanUp EXIT

# Start the interactor and the program
"${INTERACTOR[@]}" "$TEST_FILE" <"$PIPE_DIR"/0 >"$PIPE_DIR"/1 &
"${PROGRAM[@]}" >"$PIPE_DIR"/0 <"$PIPE_DIR"/1 &

# Wait for the program to finish.
TIME_COUNTER=0
while jobs %% >/dev/null 2>/dev/null; do
  sleep "$TIME_RESOLUTION"
  TIME_COUNTER=$(echo "$TIME_COUNTER"+"$TIME_RESOLUTION" | bc -l)
  # Check if the time limit has been exceeded
  if [ "$(echo "$TIME_COUNTER > $TIME_LIMIT" | bc -lq)" -eq 1 ]; then
    echo "Time Limit Exceeded ($TIME_LIMIT seconds)" >&2
    kill %% 2>/dev/null
    break
  fi
done

