#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>

using namespace eosio;

CONTRACT reporting : public contract {
  public:
    using contract::contract;
    reporting(eosio::name receiver, eosio::name code, datastream<const char*> ds):contract(receiver, code, ds) {}
  
    ACTION init();
    ACTION enrol(name user);
    ACTION appoint(name user);
    ACTION report( name reporter, std::string hash, uint64_t parentLink, bool isIncident );
    ACTION approve(uint64_t key);
    ACTION vote(uint64_t itemKey, name voter, uint64_t merit);
  
  private:
  
	  bool initialized = false;
	  
    uint64_t statusThreshold = 10;
	  uint64_t voteThreshold = 2;
	  uint64_t minConfirmations = 1;
	  uint64_t conversionFactor = 10;
    
    TABLE user {
  		name			user;
  		uint64_t		balance;
  		uint64_t 		statusR;
  		uint64_t 		statusV;
  		bool 			  verificator;
  		bool			  frozen;
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
  		uint64_t 		key;
  		uint64_t		itemKey;
  		name			  voter;
  		uint64_t		value;
		  uint64_t primary_key() const { return key; }
	  };
	  typedef eosio::multi_index<"voting"_n, voting> voting_t;
};

EOSIO_DISPATCH(reporting, (init)(enrol))
