# 生成mk文件
../configure --extra-cflags=-fPIC --extra-cxxflags=-fPIC

# 之后需要在config.mk中修改install路径
PREFIX=/home/intel/workspace/libvpx/build/install
ifeq ($(MAKECMDGOALS),dist)
DIST_DIR?=vpx-vp8-vp9-x86_64-linux-v1.9.0
else
DIST_DIR?=$(DESTDIR)/home/intel/workspace/libvpx/build/install
endif
LIBSUBDIR=lib

# 安装
make -j8
make install
