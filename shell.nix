let
  pkgs = import <nixpkgs> {};
  unstable = import (fetchTarball https://github.com/NixOS/nixpkgs-channels/archive/nixos-unstable.tar.gz) {};
  pkgs-2020-03-23 = import (fetchTarball https://github.com/NixOS/nixpkgs/archive/793c1b5c72abbfed2f98add0811a022fc713dbf3.tar.gz) {};
in
  pkgs-2020-03-23.clangStdenv.mkDerivation rec {
    pname   = "captain-ballard";
    version = "1.0.0";
    name    = "${pname}${version}";

    nativeBuildInputs = [
      pkgs.pkg-config
      unstable.cmake
    ];

    buildInputs = [
      pkgs-2020-03-23.lldb_9
      pkgs-2020-03-23.clang-tools
      pkgs-2020-03-23.clang-analyzer

      pkgs-2020-03-23.man
      pkgs-2020-03-23.man-pages
      pkgs-2020-03-23.clang-manpages
      pkgs-2020-03-23.posix_man_pages

      pkgs-2020-03-23.libgit2
      pkgs-2020-03-23.openssl
      pkgs-2020-03-23.zlib

      pkgs-2020-03-23.sqlite
      pkgs-2020-03-23.libappindicator
      pkgs-2020-03-23.fontconfig

      pkgs-2020-03-23.gtk3
      pkgs-2020-03-23.pcre
      pkgs-2020-03-23.harfbuzz
      pkgs-2020-03-23.xorg.libpthreadstubs
      pkgs-2020-03-23.xorg.libXdmcp
      pkgs-2020-03-23.utillinux
      pkgs-2020-03-23.libselinux
      pkgs-2020-03-23.libsepol
      pkgs-2020-03-23.libxkbcommon
      pkgs-2020-03-23.epoxy
      pkgs-2020-03-23.at_spi2_core.dev
      pkgs-2020-03-23.dbus
      pkgs-2020-03-23.xorg.libXtst

      pkgs-2020-03-23.glib
    ];

    FONTCONFIG_FILE = "${pkgs-2020-03-23.fontconfig.out}/etc/fonts/fonts.conf";
    LOCALE_ARCHIVE = "${pkgs-2020-03-23.glibcLocales}/lib/locale/locale-archive";

    shellHook = ''
      export NAME=${pname}
      export CMAKE=${unstable.cmake}/bin/cmake
      export CLANG=${pkgs-2020-03-23.clang}/bin/clang
      export CLANGXX=${pkgs-2020-03-23.clang}/bin/clang++
      export CLANGD=${pkgs-2020-03-23.clang-tools}/bin/clangd
      export LLDB_VSCODE=${pkgs-2020-03-23.lldb_9}/bin/lldb-vscode
    '';
  }
