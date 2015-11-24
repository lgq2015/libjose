#include <libjose/jwa_rsa.hpp>
#include <libjose/jwa.hpp>
#include <boost/bimap/unordered_set_of.hpp>
#include <boost/optional.hpp>
#include "utility.hpp"

namespace JOSE {
namespace {

struct Key {
    enum Type {
            crv,
            x,
            y,
            d,
    };
    static const boost::bimap<Key::Type, boost::bimaps::unordered_set_of<std::string>> lookup;
    static boost::optional<const std::string &> type2key(Type type) {
        auto i = lookup.left.find(type);
        if (i == lookup.left.end()) {
            return boost::none;
        }
        return i->second;
    }
    static boost::optional<Type> key2type(const std::string &key) {
        auto i = lookup.right.find(key);
        if (i == lookup.right.end()) {
            return boost::none;
        }
        return i->second;
    }
};

const boost::bimap<Key::Type, boost::bimaps::unordered_set_of<std::string>> Key::lookup = make_bimap<Key::Type, boost::bimaps::unordered_set_of<std::string>>({
    {Key::crv, "crv"},
    {Key::x, "x"},
    {Key::y, "y"},
    {Key::d, "d"},
});

struct JWA_ECImpl {
    std::shared_ptr<rapidjson::Document> doc;
    std::string crv, x, y, d;
    JWA_ECImpl(): doc{new rapidjson::Document} {}
    JWA_ECImpl(const std::string &json): doc{new rapidjson::Document} {
        doc->Parse(json.c_str());
    }
    JWA_ECImpl(void *_): doc{*reinterpret_cast<std::shared_ptr<rapidjson::Document>*>(_)} {}
    bool parse() {
        for (rapidjson::Value::ConstMemberIterator i = doc->MemberBegin(); i != doc->MemberEnd(); ++i) {
            auto key = Key::key2type(i->name.GetString());
            if (!key) {
                continue;
            }
                if (!i->value.IsString()) {
                    return false;
                }
            switch (*key) {
                case Key::crv:
                    crv = i->value.GetString();
                    break;
                case Key::x:
                    x = i->value.GetString();
                    break;
                case Key::y:
                    y = i->value.GetString();
                    break;
                case Key::d:
                    d = i->value.GetString();
                    break;
            }
        }
        return true;
    }
};

static inline const JWA_ECImpl *impl(const void * _) {
    return reinterpret_cast<const JWA_ECImpl*>(_);
}

static inline JWA_ECImpl *impl(void * _) {
    return reinterpret_cast<JWA_ECImpl*>(_);
}

} // namespace

JWA_EC::JWA_EC(): _{new JWA_ECImpl}, valid_{false} {
}

JWA_EC::JWA_EC(const std::string &json): _{new JWA_ECImpl{json}}, jose_{&impl(_)->doc}, valid_{false}  {
    init_();
}

JWA_EC::JWA_EC(void *_): _{new JWA_ECImpl{_}}, jose_{&impl(_)->doc}, valid_{false}  {
    init_();
}

void JWA_EC::init_() {
    if (!jose_) {
        return;
    }
    valid_ = impl(_)->parse();
}

JWA_EC::~JWA_EC() {
    delete impl(_);
}

std::string JWA_EC::to_pem() const {
    return std::string{};
}

} // namespace JOSE
