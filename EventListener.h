#include <functional>
#include <iterator>
#include <list>
#include <unordered_map>
#include <utility>

/*
\brief 
Event 에 대해 동작하는 함수 리스너를 저장합니다.
리스너의 인자 형식은 const Args&... 입니다.

*/
template<typename Event, typename... Args>
class EventListener {
public:
	using listener = std::function<void(const Args&...)>;
	using listener_list = std::list<listener>;
	using listener_list_iterator = typename listener_list::iterator;

	using mapped_listener = std::unordered_map<Event, listener_list>;
	using mapped_listener_iterator = typename mapped_listener::iterator;

	using listener_id = std::pair<mapped_listener_iterator, listener_list_iterator>;

	EventListener() = default;

	/*
	event_id: 함수를 실행할 이벤트의 ID 입니다.
	func: event_id 가 발생했을때 실행할 함수 입니다.

	반환값 listener_id: 현재 추가한 리스너에 대한 ID 입니다. 추가한 리스너 제거에 사용됩니다.
	*/
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

	/*
	추가한 리스너를 제거합니다.

	listener_id: 추가할 때 얻은 리스너의 ID
	*/
	void Remove(listener_id id) {
		auto& map_it = id.first;
		auto& list_it = id.second;
		map_it->second.erase(list_it);
	}

	/*
	추가된 리스너들을 실행합니다.

	event_id: 발생시킬 이벤트
	args...: 인자로 전달할 함수
	*/
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