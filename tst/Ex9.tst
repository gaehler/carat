#!/bin/sh
set -e

echo "### Test Ex9-1"
../bin/Bravais_grp Ex9_g
echo "### Ex9-1 return code $?"

echo "### Test Ex9-2"
../bin/Bravais_grp Ex9_h
echo "### Ex9-2 return code $?"

echo "### Test Ex9-3"
../bin/Is_finite Ex9_g
echo "### Ex9-3 return code $?"

echo "### Test Ex9-4"
../bin/Is_finite Ex9_h
echo "### Ex9-4 return code $?"

echo "### Test Ex9-5"
../bin/Bravais_inclusions -S Ex9_gb
echo "### Ex9-5 return code $?"

echo "### Test Ex9-6"
../bin/Bravais_inclusions -S Ex9_hb
echo "### Ex9-6 return code $?"
