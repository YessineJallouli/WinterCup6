#!/bin/bash

DST_DIR=$1
if [ -z "$DST_DIR" ]; then
	echo "Usage: $0 WEB_PUBLIC_DIR"
	exit 1
fi
CUR_DIR=$(pwd)
MJ_INSTALL_TMP_DIR=$(mktemp -d)
cd $MJ_INSTALL_TMP_DIR || (echo "Failed to create temp dir" && exit 1)
curl -L https://github.com/mathjax/MathJax/archive/master.zip -o mathjax.zip
unzip mathjax.zip || (echo "Failed to unzip MathJax" && exit 1)
cd "$CUR_DIR"
cp -r "$MJ_INSTALL_TMP_DIR/MathJax-master/es5/." "$DST_DIR/mathjax"
rm -rf $MJ_INSTALL_TMP_DIR
