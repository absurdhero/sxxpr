#include <sxx/sexpr.hpp>

namespace sxx {
	const Sexpr Sexpr::empty = Sexpr();
	const Sexpr::ptr List::null = std::make_shared<Sexpr>();

    bool Sexpr::pair::operator==(const Sexpr::pair& rhs) const {
        return *car == *rhs.car && *cdr == *rhs.cdr;
    }

	std::string Sexpr::to_text() const {
		std::string text;

		bool outer_sexpr = is_pair();

		if (outer_sexpr) {
			text = '(';
		}

		const Sexpr *current = this;

		while (true) {
			if (current->is_pair()) {
				text += current->first()->to_text();
				current = current->rest().get();
                if (current->is_empty()) {
                    break;
                }
                else if (current->is_pair())
					text += ' ';
				else
					text += " . ";

                continue;
			}
			else if (current->is_bool()) {
				if (current->as_bool())
					text += "#t";
				else
					text += "#f";
			}
			else if (current->is_string()) {
				text += "\"";
				text += current->as_string();
				text += "\"";
			}
			else if (current->is_symbol()) {
				text += current->as_symbol().c_str();
            }
			else if (current->value.type() == typeid(double)) {
				text += std::to_string(any_cast<double>(current));
			}
			else if (current->value.type() == typeid(int64_t)) {
				text += std::to_string(any_cast<int64_t>(current));
            }
            else if (current->value.type() == typeid(void*)) {
				text += "<pointer>";
			}
            else {
                text += "<unknown:";
                text += current->value.type().name();
                text += ">";
            }
            break;
		}

		if (outer_sexpr) {
			text += ')';
		}

		return text;
	}

    bool Sexpr::operator==(const Sexpr& rhs) const {
        if (is_pair()) {
            return rhs.is_pair() && as_pair() == rhs.as_pair();
        } else if (is_empty()) {
            return rhs.is_empty();
        } else if (is_string()) {
            return rhs.is_string() && as_string() == rhs.as_string();
        } else if (is_symbol()) {
            return rhs.is_symbol() && as_symbol() == rhs.as_symbol();
        }

        return rhs.value == value;
    }

	Sexpr::ptr Sexpr::first() const {
		try {
			return any_cast<Sexpr::pair>(value).car;
		}
		catch (bad_any_cast &) {
			throw new SexprTypeException();
		}
	}

	Sexpr::ptr Sexpr::rest() const {
		try {
			return any_cast<Sexpr::pair>(value).cdr;
		}
		catch (bad_any_cast &) {
			throw new SexprTypeException();
		}
	}

	void Sexpr::set_first(Sexpr *first) {
		try {
			return any_cast<Sexpr::pair>(value).car.reset(first);
		}
		catch (bad_any_cast &) {
			throw new SexprTypeException();
		}
	}

	void Sexpr::set_rest(Sexpr *rest) {
		try {
			return any_cast<Sexpr::pair>(value).cdr.reset(rest);
		}
		catch (bad_any_cast &) {
			throw new SexprTypeException();
		}
	}
}
