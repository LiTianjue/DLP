#!/bin/sh

current_pwd=`pwd`

build_root=`pwd`/output

install_root=${build_root}/output

mxml_output=${install_root}/mxml
rabbitmq_output=${install_root}/rabbitmq-c

die() {
	local m="$1"
	echo "FATAL: ${m}" >&2
	exit 1
}

mkdir -p ${build_root}
cd ${current_pwd}/package
tar -xvf mini-xml.tar.gz -C ${build_root}
tar -xvf rabbitmq-c.tar.gz -C ${build_root}


cd ${build_root}/mini-xml
./configure --prefix=${mxml_output} || die "xml config fail"
make || die "xml make"
make install


cd ${build_root}/rabbitmq-c
./configure --prefix=${rabbitmq_output} --enable-static || die "rabbit config fail"
make || die "rabbit make error"
make install

echo "--------------------"
echo " build success"
echo "--------------------"
