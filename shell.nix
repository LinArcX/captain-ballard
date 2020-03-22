let
  unstable = import (fetchTarball https://github.com/NixOS/nixpkgs-channels/archive/nixos-unstable.tar.gz) {  };
  pkgs = import <nixpkgs> {};
in
  pkgs.clangStdenv.mkDerivation rec {
    pname   = "captain-ballard";
    version = "1.0.0";
    name    = "${pname}${version}";

    nativeBuildInputs = [
      pkgs.pkg-config
      pkgs.cmake
    ];

    buildInputs = [
      pkgs.clang-tools
      pkgs.clang-analyzer

      pkgs.man
      pkgs.man-pages
      pkgs.clang-manpages
      pkgs.posix_man_pages

      unstable.libgit2
      unstable.openssl
      unstable.zlib

      pkgs.sqlite
      pkgs.libappindicator
      pkgs.fontconfig

      unstable.gtk3
      unstable.pcre
      unstable.harfbuzz
      unstable.xorg.libpthreadstubs
      unstable.xorg.libXdmcp
      unstable.utillinux
      unstable.libselinux
      unstable.libsepol
      unstable.libxkbcommon
      unstable.epoxy
      unstable.at_spi2_core.dev
      unstable.dbus
      unstable.xorg.libXtst

      unstable.glib
    ];

    FONTCONFIG_FILE = "${pkgs.fontconfig.out}/etc/fonts/fonts.conf";
    LOCALE_ARCHIVE = "${pkgs.glibcLocales}/lib/locale/locale-archive";

    shellHook = ''
      export NAME=${pname}
      export CMAKE=${pkgs.cmake}/bin/cmake
      export CLANG=${pkgs.clang}/bin/clang
      export CLANGXX=${pkgs.clang}/bin/clang++
      export CLANGD=${pkgs.clang-tools}/bin/clangd
    '';

  }
