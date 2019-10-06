# An EOS smart contract for sharing threat intelligence data (part of a master thesis)
This smart contract is part of a master thesis. 
The idea is based on the German "IT-Sicherheitsgesetz" and shall encourage critical infrastructures to share threat intelligence data in a decentralized system.
Implications of game theory and shortcomings in efficiency of a decentralized blockchain are considered.
Note that the code in this repository is used to simulate a system for the master thesis and is not deployed in the wild.

## Getting Started
Clone this repository and install [EOS Studio](https://www.eosstudio.io/).

Afterwards start an EOS node with EOS Studio. Consider the latest [tutorials](https://www.eosstudio.io/) of EOS Studio (scroll down to the bottom of the page).

Compile the smart contract, create a user _reporting_ and deploy the WASM and ABI to the _reporting_ user. The reporting user has to execute the _init()_ method.
Then, create the test users _bsi_, _bzz_, _ccc_, _delekom_, _ion_, _nsa_ and _oone_ for the simulation.

For an easy usage consider to download and install the [frontend application](https://github.com/ThomasBaumer/reporting-nodejs) of the smart contract.

Have fun! :)

## Contribution
No further implementation is considered since its a master thesis.

## Author
Thomas Baumer

## License
This project is licensed under the MIT License - see the LICENSE file for details

## Acknowledgments
Florian Menges, Benedikt Putz
