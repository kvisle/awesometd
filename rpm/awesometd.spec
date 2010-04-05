Name:           awesometd
Version:        0.4
Release:        1%{?dist}
Summary:        Awesome Tower Defense

Group:          Amusements/Games
License:        GPLv2
URL:            http://trygvevea.com/
Source0:        http://trygvevea.com/files/awesometd-2010-04-04-src.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:  SDL-devel, SDL_ttf-devel, SDL_image-devel, glib2-devel
Requires:       SDL, SDL_ttf, SDL_image, glib2

%description
Awesome Tower Defense is one of Trygve Vea's pet projects. It is written in
C/SDL, because C is awesome and SDL is easy. The combination makes it pretty
easy to port the game into different platforms.

%prep
%setup -q -n %{name}-%{version}-devel


%build
%configure
make %{?_smp_mflags}


%install
rm -rf %{buildroot}
make install DESTDIR=%{buildroot}


%clean
rm -rf %{buildroot}


%files
%defattr(-,root,root,-)
%doc AUTHORS README TODO
%{_datadir}/%{name}
%{_bindir}/%{name}


%changelog
* Mon Apr  5 2010 Ingvar Hagelund <ingvar@redpill-linpro.com> 0.4-1
- First cut of awsometd
