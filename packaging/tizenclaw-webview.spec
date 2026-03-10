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
BuildRequires:  pkgconfig(efl-extension)
BuildRequires:  pkgconfig(vc-webview)
BuildRequires:  pkgconfig(capi-system-system-settings)
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
/usr/apps/org.tizen.tizenclaw-webview/bin/tizenclaw-webview
/usr/apps/org.tizen.tizenclaw-webview/res/open_url.action
/usr/apps/org.tizen.tizenclaw-webview/res/open_dashboard.action
/usr/share/packages/org.tizen.tizenclaw-webview.xml
