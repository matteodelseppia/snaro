#include "snaro/bentley_ottmann.hpp"
#include "snaro/details/bentley_ottmann_consumer.hpp"
#include "snaro/details/sweep_line.hpp"

namespace snaro {

std::vector<bentley_ottmann::intersection>
bentley_ottmann::operator()(const std::vector<ursegment>& segments) const {
  details::sweep_line line_sweeping(segments, m_precision);
  details::bentley_ottmann_consumer consumer(m_precision);
  line_sweeping.sweep<details::bentley_ottmann_consumer>(consumer);
  const auto& intersections_vec = consumer.get_intersections();
  std::vector<bentley_ottmann::intersection> result;
  result.reserve(intersections_vec.size());
  for (const auto& intersection: intersections_vec) {
    result.emplace_back(intersection.segment, intersection.intersected.value(),
                        intersection.position);
  }
  return result;
}

} // namespace snaro
