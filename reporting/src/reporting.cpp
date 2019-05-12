#include <reporting.hpp>

ACTION reporting::init() {
		require_auth( _self );
		check ( !(initialized), "Initialization already done.");
		user_t users( _self, _self.value );
		users.emplace(_self, [&]( auto& row ) { 
		  row.user = "oone"_n; 
		  row.balance = 10; 
		  row.statusR = 10; 
		  row.statusV = 0; 
		  row.blames = 0; 
		  row.verificator = 1; 
		  row.frozen = 0; 
		});
		users.emplace(_self, [&]( auto& row ) { 
		  row.user = "delekom"_n; 
		  row.balance = 10;
		  row.statusR = 10; 
		  row.statusV = 0; 
		  row.blames = 0; 
		  row.verificator = 1; 
		  row.frozen = 0; 
		});
		users.emplace(_self, [&]( auto& row ) { 
		  row.user = "bzz"_n; 
		  row.balance = 10; 
		  row.statusR = 7; 
		  row.statusV = 0; 
		  row.blames = 0; 
		  row.verificator = 0; 
		  row.frozen = 0; 
		});
		initialized = true;
}






ACTION reporting::enrol(name user) {
  require_auth(user);
  user_t users( _self, _self.value );
	auto it = users.find(user.value);
	check(it == users.end(), "User already on the blockchain.");
	users.emplace(_self, [&]( auto& row ) { 
	  row.user = user; 
	  row.balance = 0; 
	  row.statusR = 0; 
	  row.statusV = 0; 
	  row.blames = 0; 
	  row.verificator = 0; 
	  row.frozen = 0; 
	});
}

void reporting::appoint(name user) {
	user_t users( _self, _self.value );
	auto iterator = users.find(user.value);
	check( !(iterator == users.end()), "No such user on the blockchain.");
	check( !(iterator->frozen), "This user is frozen.");
	check( iterator->statusR >= statusThreshold, "The user did not earn enough status points to become a verificator.");
	users.modify(iterator, _self, [&]( auto& row ) { 
	  row.verificator = 1; 
	});
}







ACTION reporting::report(name reporter, std::string data, uint64_t parentLink, bool isIncident) {
	require_auth( reporter );
	user_t users( _self, _self.value );
	auto it_reporter = users.find(reporter.value);
	check( !(it_reporter == users.end()), "No such user on the blockchain.");
	check( !(it_reporter->frozen), "This user is frozen.");
	
	char cdata[data.size() + 1];
	std::copy(data.begin(), data.end(), cdata);
	cdata[data.size()] = '\0';
	checksum256 hash = eosio::sha256(cdata, data.size());

	item_t item( _self, _self.value );
	for(auto& row : item) { 
	  check( !(row.hash == hash), "That item was already uploaded." ); 
	}
	item.emplace(_self, [&]( auto& row ) { 
	  row.key = item.available_primary_key(); 
	  row.parentLink = parentLink; 
	  row.reporter = reporter; 
	  row.hash = hash; 
	  row.incident = isIncident; 
	  row.voteable = 1;  
	  row.approval = 0;  
	  row.confirmations = 0;  
	  row.votes = 0;  
	  row.rating = 0; 
	});
}

ACTION reporting::approve(uint64_t key) {
		require_auth( "bsi"_n );
		item_t item( _self, _self.value );
		auto iterator = item.find(key);
		check( !(iterator == item.end()), "No such item on the blockchain.");
		item.modify(iterator, _self, [&]( auto& row ) { 
		  row.approval = 1; 
		});
}

ACTION reporting::vote(uint64_t itemKey, name voter, uint64_t merit) {
	require_auth( voter );
	user_t users( _self, _self.value );
	auto it_voter = users.find(voter.value);
	check( !(it_voter->frozen) , "Your account is frozen." );
	check( it_voter->verificator, "You have no voting permission." );

	item_t item( _self, _self.value );
	auto it_item = item.find(itemKey);
	check( it_item->voteable, "You cannot vote for that." );
	check( !(it_item->reporter == voter), "You cannot vote for your own item" );

	voting_t voting( _self, _self.value );
	for(auto& row : voting) { 
	  check( !( row.itemKey == itemKey && row.voter == voter ), "You already voted for that item." ); 
	}
	
	bool voteConfirms = 1; if(merit < 1) { voteConfirms = 0; }
	bool isVoteable = 1; if (it_item->votes + 1 >= voteThreshold) { isVoteable = 0; }
	auto it_reporter = users.find(it_item->reporter.value);	
	
	users.modify(it_reporter, _self, [&]( auto& row ) { 
	  row.statusR = row.statusR + voteConfirms; 
	});
	users.modify(it_voter, _self, [&]( auto& row ) { 
	  row.statusV = row.statusV + 1; 
	});
	voting.emplace(_self, [&]( auto& row ) { 
	  row.key = voting.available_primary_key(); 
	  row.itemKey = itemKey; 
	  row.voter = voter; 
	  row.value = merit; 
	});
	item.modify(it_item, _self, [&]( auto& row ) { 
	  row.confirmations = row.confirmations + voteConfirms; 
	  row.votes = row.votes + 1; 
	  row.rating = row.rating + merit; 
	  row.voteable = isVoteable; 
	});
	if (!isVoteable && it_item->confirmations < minConfirmations) { 
	  item.modify(it_item, _self, [&]( auto& row ) { 
	    row.rating = 0; 
	  }); 
	}
	//appoint as verificator if enough status points are earned
	if(it_reporter->statusR >= statusThreshold) { 
	  reporting::appoint( it_reporter->user); 
	}
}








ACTION reporting::transfer(name from, name to, uint64_t amount) {
	require_auth( from );
	user_t users( _self, _self.value ); 
	auto it_from = users.find(from.value); 
	auto it_to = users.find(to.value);
	
	check( !(it_from == users.end()), "The sending account does not exist. Please enrol.");
	check( !(it_to == users.end()), "The receiving account does not exist. Please enrol.");
  check( !(it_from->frozen) , "The sending account is frozen." );
  check( !(it_to->frozen) , "The receiving account is frozen." );
	check( it_from->balance - amount < it_from->balance , "IntegerOverflow (sender), not enough balance.");
	check( it_to->balance + amount > it_to->balance , "IntegerOverflow (receiver).");
	check( it_from->balance - amount >= 0 , "Sender has not enough token.");
	check( !(it_from == it_to), "You have to send the token to another user.");

	users.modify(it_from, _self, [&]( auto& row ) { row.balance = row.balance - amount; });
	users.modify(it_to, _self, [&]( auto& row ) { row.balance = row.balance + amount; });
}

ACTION reporting::buy( name buyer, uint64_t itemKey ) {
	require_auth( buyer );
	user_t users( _self, _self.value );
	auto it_buyer = users.find(buyer.value);
	check( !(it_buyer == users.end()) , "Please enrol first." );
	check( !(it_buyer->frozen) , "Your account is frozen." );

	item_t item( _self, _self.value );
	auto it_item = item.find(itemKey);
	check( !(it_item == item.end()), "No such item" );
	check( !(it_item->voteable), "Voting for this item is not yet done." );
	check( !(it_item->reporter == buyer), "You cannot buy for your own item." );
	
	name seller = it_item->reporter;
	uint64_t price = it_item->rating/conversionFactor;
	
	order_t order( _self, _self.value );
	for(auto& row : order) { 
	  check( !(row.itemKey == itemKey && row.buyer == buyer), "That order was already placed." ); 
	}
	order.emplace(_self, [&]( auto& row ) { 
	  row.key = order.available_primary_key();  
	  row.itemKey = itemKey;
	  row.seller = seller;
	  row.buyer = buyer; 
	  row.received = 0; 
	});
	reporting::transfer(buyer, seller, price);
}

ACTION reporting::received( name buyer, uint64_t orderKey, bool done ) {
	require_auth( buyer );
	user_t users( _self, _self.value );
	auto it_buyer = users.find(buyer.value);
	check( !(it_buyer == users.end()) , "Please enrol first." );
	check( !(it_buyer->frozen) , "Your account is frozen." );

	order_t order( _self, _self.value );
	auto it_order = order.find(orderKey);
	check( !(it_order == order.end()), "No such order." );
	check( it_order->buyer == buyer, "That's not your order." );
	
	if (done) { 
	  order.modify(it_order, _self, [&]( auto& row ) { 
	    row.received = 1; 
	  }); 
	}
	else { 
	  std::string reason = "Order " + std::to_string(orderKey) + " not received.";
	  reporting::blameintern(buyer, it_order->seller, reason, 1);
	}
}












ACTION reporting::blame(name blamer, name blamed, std::string reason, bool freeze) {
	require_auth( blamer );
	user_t users( _self, _self.value );
	auto it_blamer = users.find(blamer.value);
	check( !(it_blamer == users.end()) , "Please enrol first." );
	check( !(it_blamer->frozen) , "Your account is frozen." );
	
	auto it_blamed = users.find(blamed.value);
	check( !(it_blamed == users.end()) , "Blamed user not enrolled." );
	
	reporting::blameintern(blamer, blamed, reason, freeze);
}

void reporting::blameintern(name blamer, name blamed, std::string reason, bool freeze) {
  blaming_t blaming( _self, _self.value );
	blaming.emplace( _self, [&]( auto& row ) { 
	  row.key = blaming.available_primary_key(); 
	  row.blamer = blamer;  
	  row.blamed = blamed; 
	  row.reason = reason; 
	  row.freeze = freeze; 
	  row.voteable = 1; 
	  row.confirmations = 0; 
	  row.votes = 0; 
	});
}

ACTION reporting::voteb(uint64_t blameKey, name voter, bool value) {
	require_auth( voter );
	user_t users( _self, _self.value );
	auto it_voter = users.find(voter.value);
	check( !(it_voter->frozen) , "Your account is frozen." );
	check( it_voter->verificator, "You have no voting permission." );

	blaming_t blaming( _self, _self.value );
	auto it_blaming = blaming.find(blameKey);
	check( it_blaming->voteable, "You cannot vote for that." );
	check( !(it_blaming->blamer == voter), "You cannot vote for your own blaming" );

	votingb_t votingb( _self, _self.value );
	for(auto& row : votingb) { 
	  check( !( row.blameKey == blameKey && row.voter == voter ), "You already voted for that item." );
	}	
	
	bool isVoteable = 1; if (it_blaming->votes + 1 >= voteThreshold) { isVoteable = 0; }
	
	votingb.emplace(_self, [&]( auto& row ) { 
	  row.key = votingb.available_primary_key(); 
	  row.blameKey = blameKey; 
	  row.voter = voter; 
	  row.value = value; 
	});
	blaming.modify(it_blaming, _self, [&]( auto& row ) { 
	  row.confirmations = row.confirmations + value; 
	  row.votes = row.votes + 1; 
	  row.voteable = isVoteable; 
	});
	if (!isVoteable && it_blaming->confirmations >= minConfirmationsBlame) { 
    auto it_blamed = users.find(it_blaming->blamed.value);
  	users.modify(it_blamed, _self, [&]( auto& row ) { 
  	  row.blames = row.blames + it_blaming->freeze; 
  	});
  	
  	if (it_blamed->blames >= blameThreshold) { 
  	  users.modify(it_blamed, _self, [&]( auto& row ) { 
  	    row.frozen = it_blaming->freeze; 
  	  }); 
  	}
	}
}


