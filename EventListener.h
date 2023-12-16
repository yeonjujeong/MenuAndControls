#include <functional>
#include <iterator>
#include <vector>
#include <unordered_map>
#include <utility>

template<typename Event, typename... Args>
class EventListener {
public:
	using listener = std::function<void(const Args&...)>;
	using listener_list = std::vector<listener>;
	using listener_list_iterator = typename listener_list::iterator;

	using mapped_listener = std::unordered_map<Event, listener_list>;
	using mapped_listener_iterator = typename mapped_listener::iterator;

	using listener_id = std::pair<mapped_listener_iterator, listener_list_iterator>;

	EventListener() = default;

	template<typename F>
	listener_id Add(Event event_id, F func) {
		auto it = listeners_.find(event_id);
		if (it == listeners_.end()) {
			auto ref = listeners_.emplace(event_id, listener_list{});
			it = ref.first;
		}
		it->second.emplace_back(std::move(func));

		return { it , std::prev(it->second.end()) };
	}

	void Remove(listener_id id) {
		auto& map_it = id.first;
		auto& list_it = id.second;
		map_it->second.erase(list_it);
	}

	void operator()(Event event_id, const Args&... args) {
		auto it = listeners_.find(event_id);
		if (it == listeners_.end())
			return;
		for (const auto& listener : it->second) {
			listener(args...);
		}
	}

private:
	mapped_listener listeners_;
};