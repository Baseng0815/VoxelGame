{
  description = "VoxelGame - a voxel engine written in C++ and OpenGL";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };

        # Runtime/build libraries the game links against.
        libs = with pkgs; [
          glfw
          glew
          libGL
          libGLU
          freetype
          assimp
          libnoise
          entt
          glm
          nlohmann_json
        ];
      in
      {
        # Build the shell with clangStdenv so CC/CXX point at clang, not gcc.
        devShells.default = (pkgs.mkShell.override { stdenv = pkgs.clangStdenv; }) {
          nativeBuildInputs = with pkgs; [
            cmake
            pkg-config
            clang-tools # clangd, clang-format, clang-tidy
          ];

          buildInputs = libs;

          shellHook = ''
            echo "VoxelGame dev shell (clang)"
            echo "Build: cmake -B build && cmake --build build"
          '';
        };
      });
}
