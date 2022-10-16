#!sh
for x in $(ipcs -m | awk 'NR>1 {print $2}'); do ipcrm -m $x; done