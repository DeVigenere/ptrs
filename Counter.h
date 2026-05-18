#pragma once

struct Counter { // каунтер, чтобы видеть сколько ссылок сильных и слабых
	int shared_counter;
	int weak_counter;
	Counter() : shared_counter(1), weak_counter(0) {};
};