# super simple build script
with import <nixpkgs> {};
  stdenv.mkDerivation {
    name = "env";
    nativeBuildInputs = [pkg-config];
    buildInputs = [
      check
    ];
  }
