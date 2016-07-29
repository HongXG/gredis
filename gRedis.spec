Name: gRedis
Version: 1.0.1
Release: 1

Summary: gRedis, C++ redis library
License: GPL
Group: Development/Libraries
Vendor: gRedis project 
Packager: HongXG <1277206558@qq.com>

Source: %name-%version.tar.gz

Prefix: %_prefix
BuildRoot: %_tmppath/%name-%version-root

%description
gRedis is a simple to use C++ Redis API providing thread-safe, cluster, flexible.
It is modeled after the Java log4j API.

%package devel
Summary: gRedis headers, static libraries
Group: Development/Libraries
Requires: hiredis

%description devel
gRedis is a simple to use C++ Redis API providing thread-safe, cluster, flexible.
It is modeled after the Java log4j API.

%prep
rm -rf $RPM_BUILD_ROOT

%setup
./configure --prefix=%{prefix} --with-working-locale CFLAGS="$RPM_OPT_FLAGS" CXXFLAGS="$RPM_OPT_FLAGS"

%build
make

%install
rm -rf $RPM_BUILD_ROOT

make PREFIX=$RPM_BUILD_ROOT%{prefix} install
mkdir -p $RPM_BUILD_ROOT%{prefix}/include/
#cp -rp include/gRedis $RPM_BUILD_ROOT%{prefix}/include/
#rm -f $RPM_BUILD_ROOT%{prefix}/include/gRedis/*.h
find %{buildroot} -name .git -type d -exec find '{}' -delete \;
find %{buildroot} -name .svn -type d -exec find '{}' -delete \;
find %{buildroot} -name "*.in" -delete


%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,755)
%attr(755,root,root) %prefix/lib/lib*.so*
%attr(644,root,root) %prefix/lib/*.a

%files devel
%defattr(-,root,root,755)
%prefix/include/gRedis/*.h
%attr(644,root,root) 
%prefix/lib/*.a
