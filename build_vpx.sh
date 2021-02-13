# generate nakefiles
cd ref/libvpx/build
../configure --extra-cflags=-fPIC --extra-cxxflags=-fPIC

# change your install path(optional), in config.mk, change as:
PREFIX=/home/intel/workspace/libvpx/build/install
ifeq ($(MAKECMDGOALS),dist)
DIST_DIR?=vpx-vp8-vp9-x86_64-linux-v1.9.0
else
DIST_DIR?=$(DESTDIR)/home/intel/workspace/libvpx/build/install
endif
LIBSUBDIR=lib

# build and install
make -j8
make install
