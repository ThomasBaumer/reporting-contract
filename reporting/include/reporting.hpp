#include <eosio/eosio.hpp>
#include <eosio/crypto.hpp>
#include <eosio/time.hpp>
#include <eosio/system.hpp>

//things are more readable with this namespace
using namespace eosio;


/**
 *              ___             _ __                    _        _              __ _             ___                     _                               _     
 *      o O O  | _ \    ___    | '_ \   ___      _ _   | |_     (_)    _ _     / _` |    o O O  / __|    ___    _ _     | |_      _ _   __ _     __     | |_   
 *     o       |   /   / -_)   | .__/  / _ \    | '_|  |  _|    | |   | ' \    \__, |   o      | (__    / _ \  | ' \    |  _|    | '_| / _` |   / _|    |  _|  
 *    TS__[O]  |_|_\   \___|   |_|__   \___/   _|_|_   _\__|   _|_|_  |_||_|   |___/   TS__[O]  \___|   \___/  |_||_|   _\__|   _|_|_  \__,_|   \__|_   _\__|  
 *   {======|_|"""""|_|"""""|_|"""""|_|"""""|_|"""""|_|"""""|_|"""""|_|"""""|_|"""""| {======|_|"""""|_|"""""|_|"""""|_|"""""|_|"""""|_|"""""|_|"""""|_|"""""| 
 *  ./o--000'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'./o--000'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-' 
 * 
 *  by Thomas Baumer - 2019
 *  thomas.baumer@stud.uni-regensburg.de
 */



CONTRACT reporting : public contract {
  public:
    using contract::contract;
    reporting(eosio::name receiver, eosio::name code, datastream<const char*> ds):contract(receiver, code, ds) {}
  
    //Actios: Used by calling the smart contract in order to modify the state. See master thesis for a easier description.
    //You could name this an API. :)
    ACTION init();
    ACTION enrol(name user, std::string publicKey);
    ACTION updatepk(name user, std::string publicKey);
    ACTION report(name reporter, checksum256 hash, uint64_t parentLink, bool isIncident, uint64_t price, uint64_t reward);
    ACTION updateprice(name reporter, uint64_t itemKey, uint64_t price);
    ACTION approve(uint64_t key);
    ACTION apply(uint64_t itemKey, name applicant);
    ACTION selectvoter(name reporter, uint64_t itemKey, uint64_t nonce);
    ACTION vote(uint64_t itemKey, name voter, uint64_t overall, uint64_t description, uint64_t service, uint64_t quality);
    ACTION transfer(name from, name to, uint64_t amount);
    ACTION blame(name blamer, name blamed, std::string reason, bool freeze);
    ACTION voteb(uint64_t blameKey, name voter, bool value);
    ACTION buy(name buyer, uint64_t itemKey);
    ACTION received(name buyer, uint64_t orderKey, bool done);
    ACTION sent(name seller, uint64_t orderKey);
  
  private:
  
    //private Methods which are only called internal by the smart contract
    void appoint(name user);
    void blameintern(name blamer, name blamed, std::string reason, bool freeze);
    uint64_t random(uint64_t seed, uint64_t a, uint64_t b);
	  
	  //a boolean for the init() method
	  bool initialized = false;
	  
	  //parameters: These parameters can be altered (or should be considered to) by anyone who deploys this smart contract
    uint64_t statusThreshold = 10;
    
    uint64_t applicationThreshold = 5;
	  uint64_t voteThreshold = 3;
	  uint64_t minConfirmations = 2;
	  
	  uint64_t blameThreshold = 1;
	  uint64_t voteThresholdBlame = 5;
	  uint64_t minConfirmationsBlame = 2;
	  
	  //tables: These lines describe the tables/datamodel of the smart contract. This is where the smart contract persists its data.
    TABLE user {
  		name			    user;
  		uint64_t		  balance;
  		uint64_t 		  statusR;
  		uint64_t 		  statusV;
  		uint64_t      blames;
  		bool 			    verificator;
  		bool			    frozen;
  		std::string   publicKey;
  		uint64_t      primary_key() const { return user.value; }
    };
    typedef eosio::multi_index<"users"_n, user> user_t;
    
    TABLE item {
  		uint64_t		  key;
  		uint64_t		  parentLink;
  		name			    reporter;
      checksum256   hash;
  		bool			    incident;
  		bool			    voteable;
  		bool          appliable;
  		bool          setvoters;
  		bool 			    approval;
  		uint64_t      applications;
  		uint64_t 		  confirmations;
  		uint64_t 		  votes;
  		uint64_t		  rating;
  		uint64_t      price;
  		uint64_t      reward;
  		uint64_t      primary_key() const { return key; }
  		checksum256   by_hash() const { return hash; }
	  };
	  typedef eosio::multi_index<"item"_n, item, eosio::indexed_by<"hash"_n, eosio::const_mem_fun<item, checksum256, &item::by_hash>>> item_t;
	  
	  TABLE application {
  		uint64_t 		      key;
  		uint64_t		      itemKey;
  		name			        applicant;
  		bool              active;
  		eosio::time_point	timestamp;
		  uint64_t          primary_key() const { return key; }
		  uint64_t          by_itemKey() const { return itemKey; }
	  };
	  typedef eosio::multi_index<"application"_n, application, eosio::indexed_by<"itemkey"_n, eosio::const_mem_fun<application, uint64_t, &application::by_itemKey>>> application_t;	

	  TABLE voting {
  		uint64_t 		  key;
  		uint64_t		  itemKey;
  		name			    voter;
  		uint64_t		  value;
		  uint64_t      primary_key() const { return key; }
	  };
	  typedef eosio::multi_index<"voting"_n, voting> voting_t;

  	TABLE blaming {
  		uint64_t		  key;
  		name			    blamer;
  		name			    blamed;
  		std::string	  reason;
  		bool          freeze;
  		bool			    voteable;
  		uint64_t 		  confirmations;
  		uint64_t 		  votes;
  		uint64_t      primary_key() const { return key; }
  	};
  	typedef eosio::multi_index<"blaming"_n, blaming> blaming_t;
  	
  	TABLE votingb {
  		uint64_t 		  key;
  		uint64_t		  blameKey;
  		name			    voter;
  		bool		      value;
  		uint64_t      primary_key() const { return key; }
  	};
  	typedef eosio::multi_index<"votingb"_n, votingb> votingb_t;
  	
  	TABLE order {
  		uint64_t 	    key;
  		uint64_t	    itemKey;
  		name          seller;
  		name			    buyer;
  		bool          sent;
  		bool			    received;
  		uint64_t      primary_key() const { return key; }
  	};
  	typedef eosio::multi_index<"order"_n, order> order_t;
};

//every ACTION has to be mentioned here to be called from outside of the smart contract
EOSIO_DISPATCH(reporting, (init) (enrol)(updatepk) (report)(updateprice)(approve)(apply)(selectvoter)(vote) (transfer)(buy)(sent)(received) (blame)(voteb))
