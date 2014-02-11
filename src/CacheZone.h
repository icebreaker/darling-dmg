#ifndef CACHEZONE_H
#define CACHEZONE_H
#include <stddef.h>
#include <stdint.h>
#include <chrono>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

template <typename A, typename B> struct std::hash<std::pair<A, B>>
{
	size_t operator()(const std::pair<A, B>& t) const
	{
		return std::hash<A>()(t.first) ^ std::hash<B>()(t.second);
	}
};

class CacheZone
{
public:
	// block size = 512
	CacheZone(size_t maxBlocks);
	
	void store(const std::string& vfile, uint64_t blockId, const uint8_t* data, size_t bytes);
	size_t get(const std::string& vfile, uint64_t blockId, uint8_t* data, size_t offset, size_t maxBytes);
	
	inline float hitRate() const { return float(m_hits) / float(m_queries); }
private:
	void evictCache();
private:
	typedef std::chrono::system_clock::time_point CacheTimestamp;
	typedef std::pair<uint64_t, std::string> CacheKey;
	typedef std::vector<uint8_t> CacheEntry;
	
	std::unordered_map<CacheKey, CacheEntry> m_cache;
	std::map<CacheTimestamp, CacheKey> m_cacheAge;
	size_t m_maxBlocks;
	uint64_t m_queries = 0, m_hits = 0;
};



#endif