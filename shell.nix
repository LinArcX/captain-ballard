with import <nixpkgs> {};

clangStdenv.mkDerivation rec {
  pname   = "captain-ballard";
  version = "1.0.0";
  name    = "${pname}${version}";

  buildInputs = [
    cmake
    clang-tools
    clang-analyzer

    man
    man-pages
    clang-manpages
    posix_man_pages

    pkg-config
    sqlite
    libappindicator

    libgit2
    openssl
    zlib

    gtk3
    gtk3-x11
    pcre
    harfbuzz
    xorg.libpthreadstubs
    xorg.libXdmcp
    mount
  ];

  LOCALE_ARCHIVE="${glibcLocales}/lib/locale/locale-archive";

  shellHook = ''
    export NAME=${pname}
    export CMAKE=${pkgs.cmake}/bin/cmake
    export CLANG=${pkgs.clang}/bin/clang
    export CLANGXX=${pkgs.clang}/bin/clang++
    export CLANGD=${pkgs.clang-tools}/bin/clangd
  '';
}
