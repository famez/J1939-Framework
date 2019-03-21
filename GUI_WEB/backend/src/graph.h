#ifndef GRAPH_H_
#define GRAPH_H_

extern "C" {

#include <libwebsockets.h>

}

namespace J1939 {
	class SPN;
};

namespace Utils {
	class TimeStamp;
};

void saveToHistory(u32 id, const J1939::SPN& spn, const Utils::TimeStamp& timestamp);

int callback_graph(struct lws *wsi, enum lws_callback_reasons reason,
		void *user, void *in, size_t len);


#endif
