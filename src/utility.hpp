#ifndef __UTILITY_HPP___
#define __UTILITY_HPP___

#include <libjose/config.hpp>
#include <initializer_list>
#include <utility>
#include <memory>
#include <boost/bimap.hpp>
#include <rapidjson/document.h>

namespace JOSE {

template<typename T1, typename T2>
static inline boost::bimap<T1, T2> make_bimap(
        const std::initializer_list<
            std::pair<typename boost::bimap<T1, T2>::left_key_type, typename boost::bimap<T1, T2>::right_key_type>
        > &initlist) {
    using bimap_type = boost::bimap<T1, T2>;
    using value_type = typename bimap_type::value_type;
    bimap_type bimap {};
    for (const auto& iter: initlist) {
        if (!bimap.insert(value_type{iter.first, iter.second}).second) {
            throw std::invalid_argument{"already mapped"};
        }
    }
    return std::move(bimap);
}

void dump_backtrace();

std::string urlsafe_base64_encode(const ustring &base64);
ustring urlsafe_base64_decode(const std::string &base64);

std::string RSAPublicKey2PEM(const ustring &n, const ustring &e);
std::string RSAPrivateKey2PEM(const ustring &n, const ustring &e, const ustring &d, const ustring &p, const ustring &q, const ustring &dp, const ustring &dq, const ustring &qi);

struct HashFunc {
    enum Type {
        NONE,
        SHA256,
        SHA384,
        SHA512,
    };
};

ustring HMAC_sign(HashFunc::Type hash, const ustring &data, const ustring &key);
bool HMAC_verify(HashFunc::Type hash, const ustring &data, const ustring &key, const ustring &signature);

ustring RSA_sign(HashFunc::Type hash, const ustring &data, const ustring &n, const ustring &e, const ustring &d, const ustring &p, const ustring &q, const ustring &dp, const ustring &dq, const ustring &qi);
bool RSA_verify(HashFunc::Type hash, const ustring &data, const ustring &n, const ustring &e, const ustring &signature);

} // namespace JOSE

#endif // __UTILITY_HPP___
