#!/bin/bash

test=testdomcount

function die { echo Failure $1: status $2 ; exit $2 ; }

pushd ${LOCAL_TMP_DIR}
  export mecpath=${PATH}
  export PATH=./:${PATH}
#  echo "DONE SETTING PATHS"
#  echo "top dir : " ${LOCAL_TOP_DIR}
  cp ${LOCAL_TOP_DIR}/src/GeometryReaders/XMLIdealGeometryESSource/test/testdomcount.sh .
# this feels like nuts:
  sed -i '{s/CMSSW_RELEASE_BASE/LOCAL_TOP_DIR/g}' testdomcount.sh 
  sed -i '{s/CMSSW_BASE/LOCAL_TOP_DIR/g}' testdomcount.sh 
#  cp ${LOCAL_TOP_DIR}/test/${SCRAM_ARCH}/DDErrorReport .
#  echo "DONE WITH COPY AND ABOUT TO SHOW PATH"
#  echo $PATH
#  echo "DONE SHOWING PATH"
  which DOMCount
  echo ${test}IdealGeometry ---------------------------------------------------------------
  ./testdomcount.sh || die "testdomcount.sh cmsIdealGeometryXML_cfi.py" $?
  echo ${test}ExtendedGeometry ---------------------------------------------------------------
  ./testdomcount.sh ${LOCAL_TOP_DIR}/src/Geometry/CMSCommonData/python/cmsExtendedGeometryXML_cfi.py || die "testdomcount.sh cmsExtendedGeometryXML_cfi.py" $?
  echo ${test}ExtendedGeometryLiMax ---------------------------------------------------------------
  ./testdomcount.sh ${LOCAL_TOP_DIR}/src/Geometry/CMSCommonData/python/cmsExtendedGeometryLiMaxXML_cfi.py || die "testdomcount.sh cmsExtendedGeometryLiMaxXML_cfi.py" $?
  export PATH=${mecpath}
popd

exit 0
