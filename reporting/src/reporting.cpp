#include <reporting.hpp>


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




/**
 * Set some test users with various permissions. 
 * These users hold the total supply of token of the smart contract.
 * The publicKey is the same for all users to allow easy switching between the users (pretty useful for development).
 * This method has to change for a more productive deployment.
 */
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
      row.publicKey =	"-----BEGIN PUBLIC KEY-----\nMIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA3CPIoTuHlhcVmmAaDPa2\n+j/sxUlwvszLN4zSFVsOb8n5O31lIAOZosNlZwvGvUFEFLDw9XHbchfxDaROpfmD\nkbaT7ePHmIZPNRduC76c5fD4h7mUA7aww/f4Wou3kfedX/gRIQiUc40rbBiwDbdl\ne24b/mQ1mnHTKPbJNuyBCEm0gB9ln7cY7gJIdPjTS9i9bcEIh0XqkQA0HQdhNFJn\nKMGZxYOY+RQ9BHxrai7Hp2FNlnOCzi/DAoPWIQWpo7NdIQDGs3aPdtWqUKLIGA3h\n5e9TfaF4/KrCvbSgLB+SiWBOJPyec1XLVIiCW99jUUHWgkPXRZz0+sRfZQ5LIfZT\nsOSZmFMAYTf1U7rTLGS26o8tKNwlS116Xxj/Raag6rpfN6fCxH/DjFXSn0wgyIAY\nNcUb3WT5bstJm4/mEgW1IKug8cTJ99JAo50yqQOurxLxeVarAwwMLWeO/pu2UGrd\n7g6skje1urd9XPrZpNLfy/6DJPGpIwsMGCprljrLpNHC01XTLPkDsbp+vMFr2DeK\n/FNfiNEZxD4yTYiSAd22Bl3CqYlotYXh05pptZUkR4dQB0VDcb60S24BRhQHvAzf\nJxupb6j42elqRU++WyDmn+LIQeNRyUJEE2KgV3EIG8YFV7WXSc2w6Rw3oEd0IHFF\nLYGkfMMPCg3nbxfHElcv388CAwEAAQ==\n-----END PUBLIC KEY-----\n";
		});
		users.emplace(_self, [&]( auto& row ) { 
		  row.user = "ion"_n; 
		  row.balance = 10;
		  row.statusR = 10; 
		  row.statusV = 0; 
		  row.blames = 0; 
		  row.verificator = 1; 
		  row.frozen = 0; 
      row.publicKey =	"-----BEGIN PUBLIC KEY-----\nMIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA3CPIoTuHlhcVmmAaDPa2\n+j/sxUlwvszLN4zSFVsOb8n5O31lIAOZosNlZwvGvUFEFLDw9XHbchfxDaROpfmD\nkbaT7ePHmIZPNRduC76c5fD4h7mUA7aww/f4Wou3kfedX/gRIQiUc40rbBiwDbdl\ne24b/mQ1mnHTKPbJNuyBCEm0gB9ln7cY7gJIdPjTS9i9bcEIh0XqkQA0HQdhNFJn\nKMGZxYOY+RQ9BHxrai7Hp2FNlnOCzi/DAoPWIQWpo7NdIQDGs3aPdtWqUKLIGA3h\n5e9TfaF4/KrCvbSgLB+SiWBOJPyec1XLVIiCW99jUUHWgkPXRZz0+sRfZQ5LIfZT\nsOSZmFMAYTf1U7rTLGS26o8tKNwlS116Xxj/Raag6rpfN6fCxH/DjFXSn0wgyIAY\nNcUb3WT5bstJm4/mEgW1IKug8cTJ99JAo50yqQOurxLxeVarAwwMLWeO/pu2UGrd\n7g6skje1urd9XPrZpNLfy/6DJPGpIwsMGCprljrLpNHC01XTLPkDsbp+vMFr2DeK\n/FNfiNEZxD4yTYiSAd22Bl3CqYlotYXh05pptZUkR4dQB0VDcb60S24BRhQHvAzf\nJxupb6j42elqRU++WyDmn+LIQeNRyUJEE2KgV3EIG8YFV7WXSc2w6Rw3oEd0IHFF\nLYGkfMMPCg3nbxfHElcv388CAwEAAQ==\n-----END PUBLIC KEY-----\n";
		});
		users.emplace(_self, [&]( auto& row ) { 
		  row.user = "delekom"_n; 
		  row.balance = 10;
		  row.statusR = 10; 
		  row.statusV = 0; 
		  row.blames = 0; 
		  row.verificator = 1; 
		  row.frozen = 0; 
      row.publicKey =	"-----BEGIN PUBLIC KEY-----\nMIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA3CPIoTuHlhcVmmAaDPa2\n+j/sxUlwvszLN4zSFVsOb8n5O31lIAOZosNlZwvGvUFEFLDw9XHbchfxDaROpfmD\nkbaT7ePHmIZPNRduC76c5fD4h7mUA7aww/f4Wou3kfedX/gRIQiUc40rbBiwDbdl\ne24b/mQ1mnHTKPbJNuyBCEm0gB9ln7cY7gJIdPjTS9i9bcEIh0XqkQA0HQdhNFJn\nKMGZxYOY+RQ9BHxrai7Hp2FNlnOCzi/DAoPWIQWpo7NdIQDGs3aPdtWqUKLIGA3h\n5e9TfaF4/KrCvbSgLB+SiWBOJPyec1XLVIiCW99jUUHWgkPXRZz0+sRfZQ5LIfZT\nsOSZmFMAYTf1U7rTLGS26o8tKNwlS116Xxj/Raag6rpfN6fCxH/DjFXSn0wgyIAY\nNcUb3WT5bstJm4/mEgW1IKug8cTJ99JAo50yqQOurxLxeVarAwwMLWeO/pu2UGrd\n7g6skje1urd9XPrZpNLfy/6DJPGpIwsMGCprljrLpNHC01XTLPkDsbp+vMFr2DeK\n/FNfiNEZxD4yTYiSAd22Bl3CqYlotYXh05pptZUkR4dQB0VDcb60S24BRhQHvAzf\nJxupb6j42elqRU++WyDmn+LIQeNRyUJEE2KgV3EIG8YFV7WXSc2w6Rw3oEd0IHFF\nLYGkfMMPCg3nbxfHElcv388CAwEAAQ==\n-----END PUBLIC KEY-----\n";
		});
		users.emplace(_self, [&]( auto& row ) { 
		  row.user = "nsa"_n; 
		  row.balance = 10;
		  row.statusR = 10; 
		  row.statusV = 0; 
		  row.blames = 0; 
		  row.verificator = 1; 
		  row.frozen = 0; 
      row.publicKey =	"-----BEGIN PUBLIC KEY-----\nMIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA3CPIoTuHlhcVmmAaDPa2\n+j/sxUlwvszLN4zSFVsOb8n5O31lIAOZosNlZwvGvUFEFLDw9XHbchfxDaROpfmD\nkbaT7ePHmIZPNRduC76c5fD4h7mUA7aww/f4Wou3kfedX/gRIQiUc40rbBiwDbdl\ne24b/mQ1mnHTKPbJNuyBCEm0gB9ln7cY7gJIdPjTS9i9bcEIh0XqkQA0HQdhNFJn\nKMGZxYOY+RQ9BHxrai7Hp2FNlnOCzi/DAoPWIQWpo7NdIQDGs3aPdtWqUKLIGA3h\n5e9TfaF4/KrCvbSgLB+SiWBOJPyec1XLVIiCW99jUUHWgkPXRZz0+sRfZQ5LIfZT\nsOSZmFMAYTf1U7rTLGS26o8tKNwlS116Xxj/Raag6rpfN6fCxH/DjFXSn0wgyIAY\nNcUb3WT5bstJm4/mEgW1IKug8cTJ99JAo50yqQOurxLxeVarAwwMLWeO/pu2UGrd\n7g6skje1urd9XPrZpNLfy/6DJPGpIwsMGCprljrLpNHC01XTLPkDsbp+vMFr2DeK\n/FNfiNEZxD4yTYiSAd22Bl3CqYlotYXh05pptZUkR4dQB0VDcb60S24BRhQHvAzf\nJxupb6j42elqRU++WyDmn+LIQeNRyUJEE2KgV3EIG8YFV7WXSc2w6Rw3oEd0IHFF\nLYGkfMMPCg3nbxfHElcv388CAwEAAQ==\n-----END PUBLIC KEY-----\n";
		});
		users.emplace(_self, [&]( auto& row ) { 
		  row.user = "ccc"_n; 
		  row.balance = 10;
		  row.statusR = 10; 
		  row.statusV = 0; 
		  row.blames = 0; 
		  row.verificator = 1; 
		  row.frozen = 0; 
      row.publicKey =	"-----BEGIN PUBLIC KEY-----\nMIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA3CPIoTuHlhcVmmAaDPa2\n+j/sxUlwvszLN4zSFVsOb8n5O31lIAOZosNlZwvGvUFEFLDw9XHbchfxDaROpfmD\nkbaT7ePHmIZPNRduC76c5fD4h7mUA7aww/f4Wou3kfedX/gRIQiUc40rbBiwDbdl\ne24b/mQ1mnHTKPbJNuyBCEm0gB9ln7cY7gJIdPjTS9i9bcEIh0XqkQA0HQdhNFJn\nKMGZxYOY+RQ9BHxrai7Hp2FNlnOCzi/DAoPWIQWpo7NdIQDGs3aPdtWqUKLIGA3h\n5e9TfaF4/KrCvbSgLB+SiWBOJPyec1XLVIiCW99jUUHWgkPXRZz0+sRfZQ5LIfZT\nsOSZmFMAYTf1U7rTLGS26o8tKNwlS116Xxj/Raag6rpfN6fCxH/DjFXSn0wgyIAY\nNcUb3WT5bstJm4/mEgW1IKug8cTJ99JAo50yqQOurxLxeVarAwwMLWeO/pu2UGrd\n7g6skje1urd9XPrZpNLfy/6DJPGpIwsMGCprljrLpNHC01XTLPkDsbp+vMFr2DeK\n/FNfiNEZxD4yTYiSAd22Bl3CqYlotYXh05pptZUkR4dQB0VDcb60S24BRhQHvAzf\nJxupb6j42elqRU++WyDmn+LIQeNRyUJEE2KgV3EIG8YFV7WXSc2w6Rw3oEd0IHFF\nLYGkfMMPCg3nbxfHElcv388CAwEAAQ==\n-----END PUBLIC KEY-----\n";
		});
		users.emplace(_self, [&]( auto& row ) { 
		  row.user = "bzz"_n; 
		  row.balance = 10; 
		  row.statusR = 7; 
		  row.statusV = 0; 
		  row.blames = 0; 
		  row.verificator = 0; 
		  row.frozen = 0; 
      row.publicKey =	"-----BEGIN PUBLIC KEY-----\nMIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA3CPIoTuHlhcVmmAaDPa2\n+j/sxUlwvszLN4zSFVsOb8n5O31lIAOZosNlZwvGvUFEFLDw9XHbchfxDaROpfmD\nkbaT7ePHmIZPNRduC76c5fD4h7mUA7aww/f4Wou3kfedX/gRIQiUc40rbBiwDbdl\ne24b/mQ1mnHTKPbJNuyBCEm0gB9ln7cY7gJIdPjTS9i9bcEIh0XqkQA0HQdhNFJn\nKMGZxYOY+RQ9BHxrai7Hp2FNlnOCzi/DAoPWIQWpo7NdIQDGs3aPdtWqUKLIGA3h\n5e9TfaF4/KrCvbSgLB+SiWBOJPyec1XLVIiCW99jUUHWgkPXRZz0+sRfZQ5LIfZT\nsOSZmFMAYTf1U7rTLGS26o8tKNwlS116Xxj/Raag6rpfN6fCxH/DjFXSn0wgyIAY\nNcUb3WT5bstJm4/mEgW1IKug8cTJ99JAo50yqQOurxLxeVarAwwMLWeO/pu2UGrd\n7g6skje1urd9XPrZpNLfy/6DJPGpIwsMGCprljrLpNHC01XTLPkDsbp+vMFr2DeK\n/FNfiNEZxD4yTYiSAd22Bl3CqYlotYXh05pptZUkR4dQB0VDcb60S24BRhQHvAzf\nJxupb6j42elqRU++WyDmn+LIQeNRyUJEE2KgV3EIG8YFV7WXSc2w6Rw3oEd0IHFF\nLYGkfMMPCg3nbxfHElcv388CAwEAAQ==\n-----END PUBLIC KEY-----\n";
		});
		users.emplace(_self, [&]( auto& row ) { 
		  row.user = "bsi"_n; 
		  row.balance = 0; 
		  row.statusR = 0; 
		  row.statusV = 0; 
		  row.blames = 0; 
		  row.verificator = 0; 
		  row.frozen = 0; 
      row.publicKey =	"-----BEGIN PUBLIC KEY-----\nMIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA3CPIoTuHlhcVmmAaDPa2\n+j/sxUlwvszLN4zSFVsOb8n5O31lIAOZosNlZwvGvUFEFLDw9XHbchfxDaROpfmD\nkbaT7ePHmIZPNRduC76c5fD4h7mUA7aww/f4Wou3kfedX/gRIQiUc40rbBiwDbdl\ne24b/mQ1mnHTKPbJNuyBCEm0gB9ln7cY7gJIdPjTS9i9bcEIh0XqkQA0HQdhNFJn\nKMGZxYOY+RQ9BHxrai7Hp2FNlnOCzi/DAoPWIQWpo7NdIQDGs3aPdtWqUKLIGA3h\n5e9TfaF4/KrCvbSgLB+SiWBOJPyec1XLVIiCW99jUUHWgkPXRZz0+sRfZQ5LIfZT\nsOSZmFMAYTf1U7rTLGS26o8tKNwlS116Xxj/Raag6rpfN6fCxH/DjFXSn0wgyIAY\nNcUb3WT5bstJm4/mEgW1IKug8cTJ99JAo50yqQOurxLxeVarAwwMLWeO/pu2UGrd\n7g6skje1urd9XPrZpNLfy/6DJPGpIwsMGCprljrLpNHC01XTLPkDsbp+vMFr2DeK\n/FNfiNEZxD4yTYiSAd22Bl3CqYlotYXh05pptZUkR4dQB0VDcb60S24BRhQHvAzf\nJxupb6j42elqRU++WyDmn+LIQeNRyUJEE2KgV3EIG8YFV7WXSc2w6Rw3oEd0IHFF\nLYGkfMMPCg3nbxfHElcv388CAwEAAQ==\n-----END PUBLIC KEY-----\n";
		});
		initialized = true;
}





/**
 * Register new users which are not intially set by the init() method.
 * The user is not frozen and is no verificator. His balance is 0.
 * However, the user has to his publicKey.
 */
ACTION reporting::enrol(name user, std::string publicKey) {
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
	  row.publicKey = publicKey;
	});
}
/**
 * Method for updating the publicKey of an user. Nothing special to expect.
 */
ACTION reporting::updatepk(name user, std::string publicKey) {
  require_auth(user);
  user_t users( _self, _self.value );
	auto iterator = users.find(user.value);
	check( !(iterator == users.end()), "No such user on the blockchain.");
	check( !(iterator->frozen), "This user is frozen.");
	users.modify(iterator, _self, [&]( auto& row ) { 
	  row.publicKey = publicKey;
	});
}
/**
 * Private Method for appoint a user to grant him the verificator permission. 
 * This Method is called by another method. 
 * The user has to have earned enough status points by reporting data to the Incident Reporting Plattform.
 */
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






/**
 * Somewhat the core of the smart contract.
 * With this method the user submits a hash of his threat intelligence data and some additional metadata.
 * If the reward is set the user has to pay it by this method. 
 */
ACTION reporting::report(name reporter, checksum256 hash, uint64_t parentLink, bool isIncident, uint64_t price, uint64_t reward) {
	require_auth( reporter );
	user_t users( _self, _self.value );
	auto it_reporter = users.find(reporter.value);
	check( !(it_reporter == users.end()), "No such user on the blockchain.");
	check( !(it_reporter->frozen), "This user is frozen.");
	check( (reward * voteThreshold <= it_reporter->balance), "You don't own enough token to pay for the reward.");
	
	users.modify(it_reporter, _self, [&]( auto& row ) { 
	  row.balance = row.balance - reward * voteThreshold; 
	});

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
	  row.voteable = 0;  
	  row.appliable = 1;
	  row.setvoters = 0;
	  row.approval = 0;
	  row.applications = 0;
	  row.confirmations = 0;  
	  row.votes = 0;  
	  row.rating = 0; 
	  row.price = price;
	  row.reward = reward * voteThreshold;
	});
}
/**
 * Allows the user to update the price for the threat intelligence data.
 * Nothing special to expect.
 */
ACTION reporting::updateprice(name reporter, uint64_t itemKey, uint64_t price) {
  require_auth( reporter );
	user_t users( _self, _self.value );
	auto it_reporter = users.find(reporter.value);
	check( !(it_reporter == users.end()), "No such user on the blockchain.");
	check( !(it_reporter->frozen), "This user is frozen.");
  
  item_t item( _self, _self.value );
	auto it_item = item.find(itemKey);
	check( !(it_item == item.end()), "No such item on the blockchain.");
	check( it_item->reporter == reporter, "Cannot set price for items owned by other users.");
	item.modify(it_item, _self, [&]( auto& row ) { 
	  row.price = price;
	});
}
/**
 * This method may be used by the user "bsi" to approve the threat intelligence data.
 * With this method the bsi confirms the execution of the legal duty of the incident reporter. 
 * Additionally this is a sign for the validy fo the threat intelligence data.
 */
ACTION reporting::approve(uint64_t key) {
		require_auth( "bsi"_n );
		item_t item( _self, _self.value );
		auto iterator = item.find(key);
		check( !(iterator == item.end()), "No such item on the blockchain.");
		item.modify(iterator, _self, [&]( auto& row ) { 
		  row.approval = 1; 
		});
}




/**
 * Manages the application of an user to vote for the threat intelligence data.
 * The last applications closes the threat intelligence data for further applications.
 * Therefore the setvoters method can be called afterwards.
 */
ACTION reporting::apply(uint64_t itemKey, name applicant) {
  require_auth( applicant );
	user_t users( _self, _self.value );
	auto it_applicant = users.find(applicant.value);
	check( !(it_applicant->frozen) , "Your account is frozen." );
	check( it_applicant->verificator, "You have no voting permission." );
	
	item_t item( _self, _self.value );
	auto it_item = item.find(itemKey);
	check( !(it_item == item.end()), "No such item on the blockchain.");
	check( it_item->appliable, "Item is closed for applications.");
	
	application_t application( _self, _self.value );
	for(auto& row : application) { 
	  check( !( row.itemKey == itemKey && row.applicant == applicant ), "You already applied for that item." ); 
	}
  
  application.emplace(_self, [&]( auto& row ) { 
	  row.key = application.available_primary_key(); 
	  row.itemKey = itemKey; 
	  row.applicant = applicant; 
	  row.active = 0;
	  row.timestamp = eosio::current_time_point(); 
	});
	item.modify(it_item, _self, [&]( auto& row ) { 
	  row.applications = row.applications + 1;
	});
	
	//last application, disable this action, enable set voters
	if (it_item->applications >= applicationThreshold) {
	  item.modify(it_item, _self, [&]( auto& row ) { 
  	  row.setvoters = 1;
  	  row.appliable = 0;
  	});
	}
}
/**
 * This method selects in a pseudo-"random" way the voters for the quality assessment of the smart contract.
 * To do this it collects the active applications, assignes a "random" number to the entry and sorts the applications regarding to the number.
 * Afterwards 3 applications are selected and become active.
 */
ACTION reporting::selectvoter(name reporter, uint64_t itemKey, uint64_t nonce) {
  require_auth( reporter );
	user_t users( _self, _self.value );
	auto it_reporter = users.find(reporter.value);
	check( !(it_reporter->frozen) , "Your account is frozen." );
  
  item_t item( _self, _self.value );
	auto it_item = item.find(itemKey);
	check( !(it_item == item.end()), "No such item on the blockchain.");
	check( it_item->setvoters, "Application or Voting is open.");
	check( it_item->reporter == reporter, "This action has to be called by the reporter self.");
	
	//get applications for item
	uint64_t application_nums[applicationThreshold];
	uint64_t random_nums[applicationThreshold];
	uint64_t counter = 0;
	application_t application( _self, _self.value );
	for(auto& row : application) { 
	  if( row.itemKey == itemKey ) { 
	    eosio::microseconds difference = eosio::current_time_point() - row.timestamp;
	    random_nums[counter] = reporting::random(difference.count(), row.applicant.value, nonce);
	    application_nums[counter] = row.key;
	    counter = counter + 1;
	  }
	}
	
	//sort
	bool done = 0;
	while(!done) {
	  done = 1;
  	for(uint64_t i = 1; i < applicationThreshold; i++ ) {
  	  if (random_nums[i - 1] > random_nums[i]) {
  	    uint64_t swap1 = random_nums[i];
  	    random_nums[i] = random_nums[i - 1];
  	    random_nums[i - 1] = swap1;
  	    
  	    uint64_t swap2 = application_nums[i];
  	    application_nums[i] = application_nums[i - 1];
  	    application_nums[i - 1] = swap2;
  	    
  	    done = 0;
  	  }
  	}
	}
	//choose lowest random numbers and set as voters
	for(uint64_t i = 0; i < applicationThreshold; i++ ) {
	  print(random_nums[i], " ");
	  print(application_nums[i], " ");
	  if(i < voteThreshold) {
	    auto it_application = application.find(application_nums[i]);
  	  application.modify(it_application, _self, [&]( auto& row ) { 
    	  row.active = 1;
    	});
    	
    	//set order for consistency
    	order_t order( _self, _self.value );
    	order.emplace(_self, [&]( auto& row ) { 
    	  row.key = order.available_primary_key();  
    	  row.itemKey = itemKey;
    	  row.seller = reporter;
    	  row.buyer = it_application->applicant; 
    	  row.sent = 1;
    	  row.received = 0; 
    	});
	  }
	}
	
	//set status of item
	item.modify(it_item, _self, [&]( auto& row ) { 
  	row.setvoters = 0;
  	row.voteable = 1;
  });
	
}
/**
 * A helper method to generate a random number. The algorithm is a simple linear congruential generator (lcg).
 */
uint64_t reporting::random(uint64_t seed, uint64_t a, uint64_t b) {
  uint64_t m = 1337;
  uint64_t y = seed % (m - 1);
  a = a % (m - 1);
  b = b % (m - 1);
  
  return (a * y + b) % m;
}

/**
 * Besides the report() method is this method the second core method.
 * After the the voters are selected these call this method to submit their assessment of the threat intelligence data. 
 * The assessment is done by the arguments overall, description, service and quality in the range [0; 99].
 * The voter is rewarded by a status point for his voting and an optional reward in token (if it was set from the reporter by the report() Method with the argument price)
 * The reporter gets his reward if the quality of the threat intelligence data was sufficient. Additionally the reporter earns for a positive rating a status point for reporting. 
 * If enough status points for reporting are earned the reporter gets the verifcation permission granted.
 * The last vote closes the assessment of the threat intelligence data and cleans up.
 */
ACTION reporting::vote(uint64_t itemKey, name voter, uint64_t overall, uint64_t description, uint64_t service, uint64_t quality) {
	require_auth( voter );
	user_t users( _self, _self.value );
	auto it_voter = users.find(voter.value);
	check( !(it_voter->frozen) , "Your account is frozen." );
	check( it_voter->verificator, "You have no voting permission." );

	item_t item( _self, _self.value );
	auto it_item = item.find(itemKey);
	check( it_item->voteable, "You cannot vote for that." );
	check( !(it_item->reporter == voter), "You cannot vote for your own item." );
	check( !(it_item->appliable), "Applications are still open." );
	check( !(it_item->setvoters), "Voters are not determined." );

	voting_t voting( _self, _self.value );
	for(auto& row : voting) { 
	  check( !( row.itemKey == itemKey && row.voter == voter ), "You already voted for that item." ); 
	}
	
	bool activeVoter = 0;
	application_t application( _self, _self.value );
	for(auto& row : application) { 
	  if(row.itemKey == itemKey && row.applicant == voter && row.active == 1) {
	    activeVoter = 1;
	    break;
	  }
	}
	check(activeVoter, "You are no selected voter.");
	
	check((overall < 100 && description < 100 && service < 100 && quality < 100), "Only 2 digits per merit allowed.");
	uint64_t merit = overall * 1000000000 + description * 1000000 + service * 1000 + quality;
	
	bool voteConfirms = 1; if(overall < 1) { voteConfirms = 0; }
	bool isVoteable = 1; if (it_item->votes + 1 >= voteThreshold) { isVoteable = 0; }
	uint64_t reward_voter = it_item->reward / ( voteThreshold - it_item->votes );
	auto it_reporter = users.find(it_item->reporter.value);	
	
	users.modify(it_reporter, _self, [&]( auto& row ) { 
	  row.statusR = row.statusR + voteConfirms; 
	});
	users.modify(it_voter, _self, [&]( auto& row ) { 
	  row.statusV = row.statusV + 1; 
	  row.balance = row.balance + reward_voter;
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
	  row.reward = row.reward - reward_voter;
	});
	
	if (!isVoteable && it_item->reward > 0) {
	  //transfer back leftovers of reward
	  users.modify(it_reporter, _self, [&]( auto& row ) { 
	    row.balance = row.balance + it_item->reward; 
	  });
	  item.modify(it_item, _self, [&]( auto& row ) { 
  	  row.reward = 0;
	  });
	}
	if (!isVoteable && it_item->confirmations < minConfirmations) { 
	  //item has no sufficient quality
	  item.modify(it_item, _self, [&]( auto& row ) { 
	    row.rating = 0; 
	  }); 
	}
	//appoint as verificator if enough status points are earned
	if(it_reporter->statusR >= statusThreshold) { 
	  reporting::appoint( it_reporter->user); 
	}
}







/**
 * A method to allow transfers of token between the users. E.g. for exchanging money and token. 
 */
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

/**
 * Method to buy threat intelligence data from another user (the data has to have a positive overall rating).
 * To place the order the token have to be transfered by this method.
 */
ACTION reporting::buy( name buyer, uint64_t itemKey ) {
	require_auth( buyer );
	user_t users( _self, _self.value );
	auto it_buyer = users.find(buyer.value);
	check( !(it_buyer == users.end()) , "Please enrol first." );
	check( !(it_buyer->frozen) , "Your account is frozen." );

	item_t item( _self, _self.value );
	auto it_item = item.find(itemKey);
	check( !(it_item == item.end()), "No such item" );
	check( !(it_item->appliable), "Application for this item is not yet done." );
	check( !(it_item->voteable), "Voting for this item is not yet done." );
	check( !(it_item->reporter == buyer), "You cannot buy for your own item." );
	check( !(it_item->rating/1000000000 == 0), "This item does not meet quality requirements to be sold." );
	
	name seller = it_item->reporter;
	uint64_t price = it_item->price;
	
	order_t order( _self, _self.value );
	for(auto& row : order) { 
	  check( !(row.itemKey == itemKey && row.buyer == buyer), "That order was already placed." ); 
	}
	order.emplace(_self, [&]( auto& row ) { 
	  row.key = order.available_primary_key();  
	  row.itemKey = itemKey;
	  row.seller = seller;
	  row.buyer = buyer; 
	  row.sent = 0;
	  row.received = 0; 
	});
	if(price > 0) {
	  reporting::transfer(buyer, seller, price);
	}
}
/**
 * This method markes an order as received by the buyer.
 * If done is set false a blame will be triggered.
 */
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
/**
 * This method markes an order as sent by the seller.
 * Nothing special to expect.
 */
ACTION reporting::sent( name seller, uint64_t orderKey ) {
	require_auth( seller );
	user_t users( _self, _self.value );
	auto it_seller = users.find(seller.value);
	check( !(it_seller == users.end()) , "Please enrol first." );
	check( !(it_seller->frozen) , "Your account is frozen." );

	order_t order( _self, _self.value );
	auto it_order = order.find(orderKey);
	check( !(it_order == order.end()), "No such order." );
	check( it_order->seller == seller, "That's not your order." );

	order.modify(it_order, _self, [&]( auto& row ) { 
	  row.sent = 1; 
	}); 
}











/**
 * This method allows to complain about a behaviour of another user.
 * The reason is passed by and 128 char string. 
 * The freeze argument has to be true to freeze and false to unfreeze an user.
 */
ACTION reporting::blame(name blamer, name blamed, std::string reason, bool freeze) {
	require_auth( blamer );
	user_t users( _self, _self.value );
	auto it_blamer = users.find(blamer.value);
	check( !(it_blamer == users.end()) , "Please enrol first." );
	if(freeze) { check( !(it_blamer->frozen) , "Your account is frozen." ); }
	
	check(reason.length() < 128, "Less than 128 characters allowed.");
	
	auto it_blamed = users.find(blamed.value);
	check( !(it_blamed == users.end()) , "Blamed user not enrolled." );
	
	reporting::blameintern(blamer, blamed, reason, freeze);
}
/**
 * This method sets up the blame (internal).
 */
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
/**
 * This method allows to vote for a blaming.
 * If the blame is confirmed, the numbers of blames raises. 
 * Additionally, if enough blames are confirmed the user gets frozen for most of the methods of the smart contract and get stripped off his token.
 */
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
	
	bool isVoteable = 1; if (it_blaming->votes + 1 >= voteThresholdBlame) { isVoteable = 0; }
	
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
  	  auto it_blamer = users.find(it_blaming->blamer.value);
  	  users.modify(it_blamer, _self, [&]( auto& row ) { 
  	    row.balance = row.balance + it_blamed->balance;
  	  }); 
  	  users.modify(it_blamed, _self, [&]( auto& row ) { 
  	    row.balance = 0;
  	    row.frozen = it_blaming->freeze; 
  	  }); 
  	}
	}
}


