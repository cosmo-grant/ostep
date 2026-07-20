#!/bin/bash
# Fixed amount of CPU work, then report. Used to illustrate `nice`.
#
# `nice` only shows an effect under CPU contention, so to see it reliably:
#   - pin both jobs to one core (taskset) so they compete
#   - give them identical work
#   - use a large nice gap (0 vs 19)
# Then the lower-priority (higher nice) job reliably finishes last.
#
# How to run (Linux; needs a real Linux box or container, not macOS):
#   chmod +x spin.sh
#   taskset -c 0 nice -n 19 ./spin.sh low  &
#   taskset -c 0 nice -n 0  ./spin.sh high &
#   wait
#
# Expected output:
#   high finished
#   low finished
#
# Note: nice below 0 (raising priority) needs root; 0..19 does not.

label=$1
i=0
while [ "$i" -lt 20000000 ]; do i=$((i+1)); done
echo "$label finished"
