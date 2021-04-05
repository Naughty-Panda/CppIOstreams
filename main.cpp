//////////////////////////////////////////
//	C++ IO streams
//	Naughty Panda @ 2021
//////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cassert>
#include <regex>

//////////////////////////////////////////
//	6.1
//////////////////////////////////////////

void IntChecker() {

	std::regex r{ "([\\d]{1,5})" };
	std::cmatch m;
	std::string sInput;

	while (true) {

		std::cout << "Please enter an integer: ";
		std::cin >> sInput;

		if (std::regex_match(sInput.c_str(), m, r)) {

			std::cout << "Valid integer: " << m[0] << '\n';
			return;
		}
	}
}

//////////////////////////////////////////
//	6.2
//////////////////////////////////////////

std::ostream& endll(std::ostream& ostr) {

	ostr << "\n\n";
	ostr.flush();
	return ostr;
}

//////////////////////////////////////////
//	Blackjack
//////////////////////////////////////////

enum class ECardSuit : uint8_t {

	Clubs = 1U, Diamonds, Hearts, Spades,
	Undefined = 0U
};

enum class ECardValue : uint8_t {

	Two = 2U, Three, Four, Five, Six, Seven, Eight, Nine, Ten,
	Jack = 10U, Queen = 10U, King = 10U, Ace = 1U,
	Undefined = 0U
};

class Card {

protected:
	ECardSuit _suit{ ECardSuit::Undefined };
	ECardValue _value{ ECardValue::Undefined };
	bool _bVisible{ false };

public:
	Card(ECardSuit suit, ECardValue val) : _suit(suit), _value(val) {}

	ECardValue GetValue() const { return _value; }
	void Flip() { _bVisible = !_bVisible; }

	friend std::ostream& operator << (std::ostream& ostr, const Card& card);
};

class Hand {

protected:
	std::vector<Card*> _hand;

public:
	virtual ~Hand() {}

	void Add(Card* card);
	void Clear();
	uint8_t GetValue() const;
};

class GenericPlayer : public Hand {

protected:
	std::string _name{ "noname" };

public:
	GenericPlayer(const char* name) : _name(name) {}
	virtual ~GenericPlayer() override {}

	virtual bool IsHitting() const = 0;
	bool IsBoosted() const { return GetValue() > 21U ? true : false; }
	void Bust() const { std::cout << _name << " busted!\n"; }
};

//////////////////////////////////////////
//	6.3
//////////////////////////////////////////

class Player : public GenericPlayer {

public:
	Player(const char* name) : GenericPlayer(name) {}
	~Player() override {}

	bool IsHitting() const override;
	void Win() const { std::cout << _name << " wins!\n"; }
	void Lose() const { std::cout << _name << " loses!\n"; }
	void Push() const { std::cout << _name << " pushes!\n"; }
};

//////////////////////////////////////////
//	6.4
//////////////////////////////////////////

class House : public GenericPlayer {

public:
	House() : GenericPlayer("House") {}
	~House() override {}

	bool IsHitting() const override;
	void FlipFirstCard();
};

//////////////////////////////////////////
//	6.5
//////////////////////////////////////////

std::ostream& operator << (std::ostream& ostr, const Card& card) {

	if (!card._bVisible) {

		ostr << "XX";
		return ostr;
	}
	// TODO: FIX
	const char Values[]{ '0', 'A', '2', '3', '4', '5', '6', '7', '8', '9', '10', 'J', 'Q', 'K' };
	const char Suits[]{ 'c', 'd', 'h', 's' };
	ostr << Values[static_cast<char>(card._value)] << Suits[static_cast<char>(card._suit)];
	return ostr;
}

void Hand::Add(Card* card) {

	assert(card);

	_hand.push_back(card);
}

void Hand::Clear() {

	_hand.erase(_hand.begin(), _hand.end());
}

uint8_t Hand::GetValue() const {

	uint8_t result{ 0U };
	uint8_t AceCount{ 0U };

	for (auto card : _hand) {

		if (card->GetValue() == ECardValue::Ace)
			++AceCount;
		else
			result += static_cast<int>(card->GetValue());
	}

	// Here we check if one/first of our Aces can be 11
	// It can be 11 if our hand value <= 10 - (AceCount - 1)
	if (AceCount != 0U && result <= 21U - 11U - (AceCount - 1U)) {

		result += 11U;
		--AceCount;
	}

	// If Ace cant't be 11, it'll be 1
	// for each of Aces left in hand
	result += AceCount;

	return result;
}

//////////////////////////////////////////
//	6.3
//////////////////////////////////////////

bool Player::IsHitting() const {

	std::cout << _name << ", do you want a hit? (Y/N): ";
	char input;
	std::cin >> input;
	return (input == 'y' || input == 'Y');
}

//////////////////////////////////////////
//	6.4
//////////////////////////////////////////

bool House::IsHitting() const {

	return (GetValue() <= 16);
}

void House::FlipFirstCard() {

	if (_hand.empty()) {
		std::cout << "No cards to flip!\n";
		return;
	}

	_hand[0]->Flip();
}



int main() {

	IntChecker();

	return 0;
}
