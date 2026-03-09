Name:       tizenclaw-webview
Summary:    TizenClaw WebView Application
Version:    1.0.0
Release:    1
Group:      Applications/Connectivity
License:    Apache-2.0
URL:        https://tizen.org
Source0:    %{name}-%{version}.tar.gz
BuildRequires:  cmake
BuildRequires:  pkgconfig(elementary)
BuildRequires:  pkgconfig(ecore)
BuildRequires:  pkgconfig(ecore-evas)
BuildRequires:  pkgconfig(eina)
BuildRequires:  pkgconfig(evas)
BuildRequires:  pkgconfig(ewebkit)
BuildRequires:  pkgconfig(capi-appfw-application)
BuildRequires:  pkgconfig(capi-appfw-app-control)
BuildRequires:  pkgconfig(dlog)

%description
Standalone EFL WebView application for TizenClaw LLM agent.
It receives a URL via AppControl and renders it using EWebKit.

%prep
%setup -q

%build
cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix}
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

%files
%manifest packaging/tizenclaw-webview.manifest
%defattr(-,root,root,-)
%{_bindir}/tizenclaw-webview
/usr/share/packages/org.tizen.tizenclew-webview.xml
