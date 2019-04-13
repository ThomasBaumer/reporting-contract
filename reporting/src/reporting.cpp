#include <reporting.hpp>

ACTION reporting::init() {
		require_auth( _self );
		check ( !(initialized), "Initialization already done.");
		user_t users( _self, _self.value );
		users.emplace(_self, [&]( auto& row ) { row.user = "oone"_n;  row.balance = 10;  row.statusR = 10;  row.statusV = 0;  row.verificator = 1;  row.frozen = 0; });
		users.emplace(_self, [&]( auto& row ) { row.user = "delekom"_n;  row.balance = 10;  row.statusR = 10;  row.statusV = 0;  row.verificator = 0;  row.frozen = 0; });
		item_t item( _self, _self.value );
		item.emplace(_self, [&]( auto& row ) { row.key = 0;  row.parentLink = 0;  row.reporter = _self;  row.hash = "The Russians.";  row.incident = 1; row.voteable = 0;  row.approval = 0;  row.confirmations = 0;  row.votes = 0;  row.rating = 0; });
		voting_t voting( _self, _self.value );
		voting.emplace(_self, [&]( auto& row ) { row.key = 0; row.itemKey = 0; row.voter = _self; row.value = 0; });
		initialized = true;
}

ACTION reporting::enrol(name user) {
  require_auth(user);
  user_t users( _self, _self.value );
	auto iterator = users.find(user.value);
	if (iterator == users.end()) { users.emplace(_self, [&]( auto& row ) { row.user = user; row.balance = 0; row.statusR = 0; row.statusV = 0; row.verificator = 0; row.frozen = 0; }); }
	else { print("User already on the blockchain." ); }
}

ACTION reporting::appoint(name user) {
	require_auth( user );
	user_t users( _self, _self.value );
	auto iterator = users.find(user.value);
	check( !(iterator == users.end()), "No such user on the blockchain.");
	check( !(iterator->frozen), "Your account is frozen.");
	check( iterator->statusR >= statusThreshold, "Not enough status points earned to become a verificator.");
	users.modify(iterator, _self, [&]( auto& row ) { row.verificator = 1; });
}

ACTION reporting::report(name reporter, std::string hash, uint64_t parentLink, bool isIncident) {
	require_auth( reporter );
	item_t item( _self, _self.value );
	for(auto& row : item) { check( !(row.hash == hash), "That item was already uploaded." ); }
	item.emplace(_self, [&]( auto& row ) { row.key = item.available_primary_key(); row.parentLink = parentLink; row.reporter = reporter; row.hash = hash; row.incident = isIncident; row.voteable = 1;  row.approval = 0;  row.confirmations = 0;  row.votes = 0;  row.rating = 0; });
}

ACTION reporting::approve(uint64_t key) {
		require_auth( "bsi"_n );
		item_t item( _self, _self.value );
		auto iterator = item.find(key);
		check( !(iterator == item.end()), "No such item on the blockchain.");
		item.modify(iterator, _self, [&]( auto& row ) { row.approval = 1; });
}

ACTION reporting::vote(uint64_t itemKey, name voter, uint64_t merit) {
	require_auth( voter );
	user_t users( _self, _self.value );
	auto it_voter = users.find(voter.value);
	check( !(it_voter->frozen) , "Your account is frozen." );
	check( it_voter->verificator, "You have no voting permission." );

	voting_t voting( _self, _self.value );
	for(auto& row : voting) { check( !( row.itemKey == itemKey && row.voter == voter ), "You already voted for that item." ); }

	item_t item( _self, _self.value );
	auto it_item = item.find(itemKey);
	check( it_item->voteable, "You cannot vote for that." );
	check( !(it_item->reporter == voter), "You cannot vote for your own item" );
	
	bool voteConfirms = 1; if(merit < 1) { voteConfirms = 0; }
	bool isVoteable = 1; if (it_item->votes + 1 >= voteThreshold) { isVoteable = 0; }
	auto it_reporter = users.find(it_item->reporter.value);	
	users.modify(it_reporter, _self, [&]( auto& row ) { row.statusR = row.statusR + voteConfirms; });
	users.modify(it_voter, _self, [&]( auto& row ) { row.statusV = row.statusV + 1; });
	voting.emplace(_self, [&]( auto& row ) { row.key = voting.available_primary_key(); row.itemKey = itemKey; row.voter = voter; row.value = merit; });
	item.modify(it_item, _self, [&]( auto& row ) { row.confirmations = row.confirmations + voteConfirms; row.votes = row.votes + 1; row.rating = row.rating + merit; row.voteable = isVoteable; });
}