# What is this?
A simple RPM package for UltimMC that contains a script that downloads and installs real UltimMC on Red Hat based systems.

It contains a `.desktop` file, a `.metainfo.xml` file, an icon, and a simple script that does the heavy lifting.

# How to build this?
You need the `rpm-build` package. Switch into this directory, then run:
```
rpmbuild --build-in-place -bb MultiMC5.spec
```

Replace the version with whatever is appropriate.
