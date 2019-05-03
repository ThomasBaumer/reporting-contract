#include <eosio/eosio.hpp>

using namespace eosio;

CONTRACT reporting : public contract {
  public:
    using contract::contract;
    reporting(eosio::name receiver, eosio::name code, datastream<const char*> ds):contract(receiver, code, ds) {}
  
    ACTION init();
    ACTION enrol(name user);
    ACTION report(name reporter, std::string hash, uint64_t parentLink, bool isIncident);
    ACTION approve(uint64_t key);
    ACTION vote(uint64_t itemKey, name voter, uint64_t merit);
    ACTION transfer(name from, name to, uint64_t amount);
    ACTION blame(name blamer, name blamed, std::string reason, bool freeze);
    ACTION voteb(uint64_t blameKey, name voter, bool value);
    ACTION buy(name buyer, uint64_t itemKey);
    ACTION received(name buyer, uint64_t orderKey, bool done);
  
  private:
  
    void appoint(name user);
    void blameintern(name blamer, name blamed, std::string reason, bool freeze);
	  
	  bool initialized = false;
	  
    uint64_t statusThreshold = 10;
	  uint64_t voteThreshold = 2;
	  uint64_t blameThreshold = 1;
	  uint64_t minConfirmations = 1;
	  uint64_t minConfirmationsBlame = 2;
	  uint64_t conversionFactor = 10;
    
    TABLE user {
  		name			    user;
  		uint64_t		  balance;
  		uint64_t 		  statusR;
  		uint64_t 		  statusV;
  		uint64_t      blames;
  		bool 			    verificator;
  		bool			    frozen;
  		uint64_t primary_key() const { return user.value; }
    };
    typedef eosio::multi_index<"users"_n, user> user_t;
    
    TABLE item {
  		uint64_t		  key;
  		uint64_t		  parentLink;
  		name			    reporter;
  		std::string		hash;
  		bool			    incident;
  		bool			    voteable;
  		bool 			    approval;
  		uint64_t 		  confirmations;
  		uint64_t 		  votes;
  		uint64_t		  rating;
  		uint64_t primary_key() const { return key; }
	  };
	  typedef eosio::multi_index<"item"_n, item> item_t;
	  
	  TABLE voting {
  		uint64_t 		  key;
  		uint64_t		  itemKey;
  		name			    voter;
  		uint64_t		  value;
		  uint64_t primary_key() const { return key; }
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
  		uint64_t primary_key() const { return key; }
  	};
  	typedef eosio::multi_index<"blaming"_n, blaming> blaming_t;
  	
  	TABLE votingb {
  		uint64_t 		  key;
  		uint64_t		  blameKey;
  		name			    voter;
  		bool		      value;
  		uint64_t primary_key() const { return key; }
  	};
  	typedef eosio::multi_index<"votingb"_n, votingb> votingb_t;
  	
  	TABLE order {
  		uint64_t 	    key;
  		uint64_t	    itemKey;
  		name			    buyer;
  		bool			    received;
  		uint64_t primary_key() const { return key; }
  	};
  	typedef eosio::multi_index<"order"_n, order> order_t;
};

EOSIO_DISPATCH(reporting, (init) (enrol) (report)(approve)(vote) (transfer)(buy)(received) (blame)(voteb))
