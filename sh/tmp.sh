#!/bin/sh
set -eu
name="$(basename "$0" .sh)"

# This snippet creates a named temporary directory that is removed when the
# script exits, regardless of error status or signal that induced the exit

tmp="$(mktemp -d -t "$name.XXXXXXXXXX")"
trap 'rv=$?; rm -rf '"$tmp"'; exit $rv' EXIT
trap 'exit $?' TERM
