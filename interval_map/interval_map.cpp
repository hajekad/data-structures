#include "bits/stdc++.h"

template<typename K, typename V>
class interval_map {
	friend void IntervalMapTest();
	V m_valBegin;
	std::map<K,V> m_map;
public:
	interval_map(V const& val)
	: m_valBegin(val)
	{}

	void assign(K const& keyBegin, K const& keyEnd, V const& val) {
        if(!(keyBegin < keyEnd))
		{
			return;
		}

    	auto itLower = m_map.upper_bound(keyBegin);
		auto itUpper = m_map.upper_bound(keyEnd);
	
		if (!(itLower == m_map.begin()) && !(keyBegin < std::prev(itLower)->first) && !((std::prev(itLower)->first) < keyBegin))
		{
			itLower = std::prev(itLower);
		}

    	V valStart = (!(itLower == m_map.begin()) ? std::prev(itLower)->second : m_valBegin);
		V valEnd = (!(itUpper == m_map.end()) ? std::prev(itUpper)->second : m_valBegin);

    	if(val == m_valBegin && m_map.empty())
		{
    	    m_map.insert({keyBegin, val});
    	    m_map.insert({keyEnd, m_valBegin});
    	    return;
    	}
		else if(itLower == m_map.end() || itUpper == m_map.begin())
		{
    	    if (!(val == m_valBegin))
			{
    	        m_map.insert({keyBegin, val});
    	        m_map.insert({keyEnd, m_valBegin});
    	    }
    	    return;
    	}

    	if (!(itLower == itUpper) || !(val == valStart))
		{
			m_map.erase(itLower, itUpper);
		}

    	if (!(valStart == val))
		{
			m_map.insert({keyBegin, val});
		}

    	if (!(valEnd == val))
		{
			m_map.insert({keyEnd, valEnd});
		}
    }

	V const& operator[]( K const& key ) const {
		auto it=m_map.upper_bound(key);
		if(it==m_map.begin()) {
			return m_valBegin;
		} else {
			return (--it)->second;
		}
	}
};

void testOverlap()
{
    interval_map<int, char> map('X');
    map.assign(1, 5, 'A');
    map.assign(3, 7, 'B');
    assert(map[2] == 'A');
    assert(map[4] == 'B');
    assert(map[6] == 'B');
    std::cout << "testOverlap passed\n";
}

void testAdjacentDifferent() {
    interval_map<int, char> map('X');
    map.assign(1, 3, 'A');
    map.assign(3, 5, 'B');
    assert(map[2] == 'A');
    assert(map[3] == 'B');
    assert(map[4] == 'B');
    std::cout << "testAdjacentDifferent passed\n";
}

void testConsecutiveSameValue() {
    interval_map<int, char> map('X');
    map.assign(1, 3, 'A');
    map.assign(3, 5, 'A');
    assert(map[2] == 'A');
    assert(map[4] == 'A');
    std::cout << "testConsecutiveSameValue passed\n";
}

void testCompleteCover() {
    interval_map<int, char> map('X');
    map.assign(1, 3, 'A');
    map.assign(3, 5, 'B');
    map.assign(0, 6, 'C');
    assert(map[0] == 'C');
    assert(map[3] == 'C');
    assert(map[5] == 'C');
    std::cout << "testCompleteCover passed\n";
}

void testEmptyInterval() {
    interval_map<int, char> map('X');
    map.assign(5, 5, 'A');
    map.assign(7, 2, 'B');
    assert(map[4] == 'X');
    assert(map[6] == 'X');
    std::cout << "testEmptyInterval passed\n";
}

void testEdgeAndLargeInterval()
{
    interval_map<int, char> map('X');
    map.assign(std::numeric_limits<int>::min(), 0, 'A');
    map.assign(0, std::numeric_limits<int>::max(), 'B');
    assert(map[std::numeric_limits<int>::min()] == 'A');
    assert(map[-1] == 'A');
    assert(map[0] == 'B');
    assert(map[std::numeric_limits<int>::max() - 1] == 'B');
    std::cout << "testEdgeAndLargeInterval passed\n";
}

void testRandomizedLarge()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    const int testIterations = 1000;
    const int maxRangeLength = 100;
    const char minValue = 'A';
    const char maxValue = 'Z';
    const int keyRange = 1000;

    interval_map<int, char> map(minValue);

    std::vector<char> verificationArray(keyRange, minValue);

    for (int i = 0; i < testIterations; ++i) {
        int rangeStart = rand() % keyRange;
        int rangeLength = rand() % maxRangeLength + 1;
        int rangeEnd = std::min(rangeStart + rangeLength, keyRange);
        char value = static_cast<char>(minValue + rand() % (maxValue - minValue + 1));

        map.assign(rangeStart, rangeEnd, value);

        std::fill(verificationArray.begin() + rangeStart, verificationArray.begin() + rangeEnd, value);
    }

    for (int i = 0; i < keyRange; ++i) {
        assert(map[i] == verificationArray[i]);
    }

    std::cout << "Randomized large test passed\n";
}

void testMaxRangeAssignment() {
    interval_map<int, char> map('X');
    map.assign(std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), 'M');
    assert(map[std::numeric_limits<int>::min()] == 'M');
    assert(map[0] == 'M');
    assert(map[std::numeric_limits<int>::max() - 1] == 'M');
    std::cout << "testMaxRangeAssignment passed\n";
}

void testOverlappingMaxRange() {
    interval_map<int, char> map('X');
    map.assign(std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), 'A');
    map.assign(std::numeric_limits<int>::min() + 10, std::numeric_limits<int>::max() - 10, 'B');
    assert(map[std::numeric_limits<int>::min()] == 'A');
    assert(map[std::numeric_limits<int>::min() + 11] == 'B');
    assert(map[std::numeric_limits<int>::max() - 11] == 'B');
    assert(map[std::numeric_limits<int>::max() - 1] == 'A');
    std::cout << "testOverlappingMaxRange passed\n";
}

void testRepeatedValueOverlap() {
    interval_map<int, char> map('X');
    map.assign(1, 100, 'A');
    map.assign(50, 150, 'A');
    map.assign(100, 200, 'A');
    assert(map[1] == 'A');
    assert(map[50] == 'A');
    assert(map[150] == 'A');
    assert(map[199] == 'A');
    std::cout << "testRepeatedValueOverlap passed\n";
}

void testRandomAssignmentsStress() {
    interval_map<int, char> map('X');
    const int iterations = 10000;
    const int range = 1000;

    for(int i = 0; i < iterations; ++i) {
        int start = rand() % range;
        int end = start + (rand() % (range - start));
        char value = 'A' + (rand() % 26);
        map.assign(start, end, value);
    }

    std::cout << "testRandomAssignmentsStress passed\n";
}

signed main(void)
{
	testOverlap();
	testAdjacentDifferent();
	testConsecutiveSameValue();
	testEmptyInterval();
	testEdgeAndLargeInterval();
	testRandomizedLarge();
	testMaxRangeAssignment();
    testOverlappingMaxRange();
    testRepeatedValueOverlap();
    testRandomAssignmentsStress();

	return 0;
}
