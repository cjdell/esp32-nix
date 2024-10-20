{
  description = "Node.js TypeScript Project Flake";

  inputs = {
    nixpkgs.url = "nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
    nixpkgs-esp-dev = {
      url = "github:mirrexagon/nixpkgs-esp-dev/86a2bbe01fe0258887de7396af2a5eb0e37ac3be";
      inputs.nixpkgs.follows = "nixpkgs";
      inputs.flake-utils.follows = "flake-utils";
    };
  };

  outputs = { self, nixpkgs, flake-utils, nixpkgs-esp-dev }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
          overlays = [ (import "${nixpkgs-esp-dev}/overlay.nix") ];
        };
      in
      {
        devShell = pkgs.mkShell
          {
            name = "esp32-s3-nix";

            buildInputs = with pkgs; [
              mkspiffs-presets.esp-idf
              screen

              # esp-idf-full
              (esp-idf-esp32.override {
                rev = "632e0c2a9fc7c754db4135dabb67f7fc6aa9fb87";
                sha256 = "sha256-j3NaEzMPg2qJ37Gy1y1rdaFdR1/047jOef0Sw/XnCzc=";
                toolsToInclude = [
                  "xtensa-esp32s3-elf"
                ];
              })
            ];

            ESP_DEV_PATH = nixpkgs-esp-dev;
          };
      });
}
