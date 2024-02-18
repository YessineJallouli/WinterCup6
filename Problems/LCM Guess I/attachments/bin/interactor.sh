#!/usr/bin/env bash

function gcd() {
    ! (( $1 % $2 )) && echo "$2" || gcd "$2" $(( $1 % $2 ))
}

function lcm()
{
	X=$(( $1 * $2 ))
	Y=$(gcd "$1" "$2")
	echo "$(( X / Y ))"
}

TEST_FILE="$1"
if [ -z "$TEST_FILE" ]; then
	echo "Usage $0 TEST_FILE" 
	exit 1
fi

if [[ "$TEST_FILE" == "-" ]]; then
	{ IFS= read -r N; IFS= read -r P; }
elif ! [ -f "$TEST_FILE" ]; then
	echo "Unable to open test file $TEST_FILE" >&2
	exit 2
else
	{ IFS= read -r N; IFS= read -ra P; } < "$TEST_FILE"
fi

echo "$N"
P=(0 ${P[@]})
QUERY_COUNTER=0
while IFS=' ' read -ra input;
do
	if [ -n "$DEBUG" ]; then
		echo Received Input "${input[@]}" >>"$DEBUG"
	fi
	
	if !  echo "${input[@]}" | grep -Eq '^[\?!](\s[1-9][0-9]*)+$'; then
		echo Invalid query "${input[*]}" >&2
		exit 3
	fi
	if [ "${input[0]}" == '!' ]; then
		if [[ "${input[*]:1}" == "${P[*]:1}" ]]; then
			echo "Solution guessed the correct permutation! Number of queries: $QUERY_COUNTER" >&2
			exit 0
		else
			echo 'Solution guessed the wrong permutation!
Expected: '"${P[*]:1}"
'Found   : '"${input[*]:1}" >&2
			exit 1
		fi
	elif [ "${input[0]}" == '?' ]; then
		A=${P[${input[1]}]}
		B=${P[${input[2]}]}
		lcm "$A" "$B"
		QUERY_COUNTER=$(( QUERY_COUNTER + 1 ))
		if [ -n "$DEBUG" ]; then
			echo Sent Output "$(lcm "$A" "$B")" >>"$DEBUG"
		fi
	else
		echo Unknown command: "${input[*]}" >&2
		exit 2
	fi
done
