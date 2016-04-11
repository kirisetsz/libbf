#ifndef BF_HASH_POLICY_H
#define BF_HASH_POLICY_H

#include <functional>
#include <bf/h3.h>
#include <bf/object.h>

namespace bf {

/// The hash digest type.
typedef uint64_t digest;

/// The hash function type.
typedef std::function<digest(object const&)> hash_function;

/// A function that hashes an object *k* times.
typedef std::function<std::vector<digest>(object const&)> hasher;

class default_hash_function
{
public:
  constexpr static size_t max_obj_size = 36;

  default_hash_function(size_t seed);

  digest operator()(object const& o) const;

private:
  h3<digest, max_obj_size> h3_;
};

/// A hasher which hashes an object *k* times.
class default_hasher
{
public:
  default_hasher(std::vector<hash_function> fns);

  std::vector<digest> operator()(object const& o) const;

private:
  std::vector<hash_function> fns_;
};

/// A hasher which hashes an object two times and generates *k* digests through
/// a linear combinations of the two digests.
class double_hasher
{
public:
  double_hasher(size_t k, hash_function h1, hash_function h2);

  std::vector<digest> operator()(object const& o) const;

private:
  size_t k_;
  hash_function h1_;
  hash_function h2_;
};

/// Creates a default or double hasher with the default hash function, using
/// seeds from a linear congruential PRNG.
///
/// @param k The number of hash functions to use.
///
/// @param seed The initial seed of the PRNG.
///
/// @param double_hashing If `true`, the function constructs a ::double_hasher
/// and a ::default_hasher otherwise.
///
/// @return A ::hasher with the *k* hash functions.
///
/// @pre `k > 0`
hasher make_hasher(size_t k, size_t seed = 0, bool double_hashing = false);

} // namespace bf

#endif
