{
  description = "Node.js TypeScript Project Flake";

  inputs = {
    nixpkgs.url = "nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
    nixpkgs-esp-dev = {
      url = "github:mirrexagon/nixpkgs-esp-dev";
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
            name = "esp32-nix";

            buildInputs = with pkgs; [
              esp-idf-full
            ];

            ESP_DEV_PATH = nixpkgs-esp-dev;
          };
      });
}
