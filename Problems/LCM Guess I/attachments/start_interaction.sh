#!/usr/bin/env bash
COUNTER=0
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

if [ "$COUNTER" -ne 2 ]; then
	echo "Usage: $0 TEST_CASE @ INTERACTOR [ARGS...] @ PROGRAM [ARGS...]"
	exit 1
else
  echo "Starting Interaction with the following parameters:"
	echo "  Test Case:" "${TEST_FILE}"
	echo "  Interactor:" "${INTERACTOR[@]}"
	echo "  Program File:" "${PROGRAM[@]}"
fi

if [ -n "$NDEBUG" ]; then
	unset DEBUG
elif [ -z "$DEBUG" ]; then
	export DEBUG=interaction.txt
fi

if [ -f "$DEBUG" ]; then
	rm "$DEBUG"
fi

if [[ "$TEST_FILE" == "-" ]]; then
	IFS= read -r N
	IFS= read -r P
	INTERACTOR_TMP_FILE=$(mktemp)
	echo $N >>"$INTERACTOR_TMP_FILE" 
	echo $P >>"$INTERACTOR_TMP_FILE"
	TEST_FILE="$INTERACTOR_TMP_FILE"

fi

: | { "${INTERACTOR[@]}" "$TEST_FILE" | "${PROGRAM[@]}"; } > /dev/fd/0
if [ -n "$INTERACTOR_TMP_FILE" ]; then
	rm "$INTERACTOR_TMP_FILE"
fi
