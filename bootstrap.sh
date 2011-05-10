#!/bin/bash
IN_BOOTSTRAP=1
mkdir -p deps
cd deps/
source ../cmakecheck.sh
source ../boostcheck.sh
source ../kccheck.sh
cd ..

# rebuild the ./configure.deps script
echo "CMAKE=$cmakecmd" > configure.deps

# we add a boost root only if it is the deps directory
if [ ! -z $BOOST_ROOT ] && [ $BOOST_ROOT == $PWD/deps ]; then
  echo "BOOST_ROOT=$BOOST_ROOT" >> configure.deps
fi

# we add a boost root only if it is the deps directory
if [ ! -z $KC_ROOT ] && [ $KC_ROOT == $PWD/deps ]; then
  echo "KC_ROOT=$KC_ROOT" >> configure.deps
fi