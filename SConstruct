# -*- python -*-
#
# Setup our environment
#
import glob, os.path, sys, traceback
import lsst.SConsUtils as scons

try:
    scons.ConfigureDependentProducts
except AttributeError:
    import lsst.afw.scons.SconsUtils
    scons.ConfigureDependentProducts = lsst.afw.scons.SconsUtils.ConfigureDependentProducts

env = scons.makeEnv(
    "fitsthumb",
#    "hg",
    r"$HeadURL: FILL IN PATH /fitsthumb/trunk/SConstruct $",
    scons.ConfigureDependentProducts("fitsthumb"),
#    [
#        ["python", "Python.h"],
#        ["cfitsio", "fitsio.h", "libcfitsio.a"],        
#        ["afw", "lsst/afw/image/MaskedImage.h", "afw"],
#    ],
)

env.libs["fitsthumb"] += env.getlibs("")
# FH:
#  Currently, cfitsio is static linked.
#env.libs["fitsthumb"] += env.getlibs("cfitsio") 
#  Other linked libraries (exept gcc cores) are /usr/lib64/libjpeg.so,/usr/lib64/libpng12.so,/lib64/libz.so
#  See $ ldd _fitsThumb.so 
env.libs['fitsthumb'] += [ 'jpeg', 'png', 'z', 'cfitsio']
env.libs["fitsthumb"] += env.getlibs("afw")
#env.libs["fitsthumb"] += env.getlibs("libcfitsio.a")
#env.libs["hscdistest"] += env.getlibs("")
#env.libs["hscdistest"] += env.getlibs("gsl gslcblas afw daf_base")
#env.libs["hscdistest"] += env.getlibs("pex_exceptions afw boost utils daf_base daf_data daf_persistence pex_logging pex_policy security")
print '****envlibs****', env.libs['fitsthumb']

for d in (
#    "src",
    "lib",
#   "include",      
    "python/hsc/onsite/qa",
#    "examples",
#    "tests",
):
    SConscript(os.path.join(d, "SConscript"))

Alias("install", [
    env.Install(env['prefix'], "python"),
    env.Install(env['prefix'], "examples"),
    env.Install(env['prefix'], "lib"),
    env.InstallEups(os.path.join(env['prefix'], "ups")),
    ])

scons.CleanTree(r"*~ core *.so *.os *.o *.pyc config.log")
env['IgnoreFiles'] = r"(~$|\.pyc$|^\.svn$|\.o$)"

env.Declare()
env.Help("""
LSST Application Framework packages
""")
