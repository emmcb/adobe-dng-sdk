# This differs from the similar script in the sample builds. New symlinks are always created when
# building the frameworks, replacing anything that exists. When building the samples the symlinks
# are only created if neccessary, an existing symlink or physical copy is kept.

mkdir -p ${CLIENT_DIR}/../Frameworks

if [ -d ${CLIENT_DIR}/../Frameworks/${PRODUCT_NAME}.framework ]; then
	rm -rf ${CLIENT_DIR}/../Frameworks/${PRODUCT_NAME}.framework
fi

ln -fs ${XMPLIB_DIR}/${PRODUCT_NAME}.framework ${CLIENT_DIR}/../Frameworks/
