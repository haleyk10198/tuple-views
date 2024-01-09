//
// Created by kkwok9 on 1/9/24.
//

#ifndef TUPLE_VIEWS_TUPLE_ADAPTOR_HXX
#define TUPLE_VIEWS_TUPLE_ADAPTOR_HXX

#include <concepts>
#include <utility>

namespace ranges {
    namespace views::tuple::_adaptor {
        // mimic of gcc13 libstdc++-v3 implementation of adaptor

        template<typename F, typename... Args>
        struct partial;

        template<typename T1, typename T2>
        struct pipe;

        template<typename F, typename... Args>
        concept tuple_adaptor_invocable = std::regular_invocable<F, Args...>;

        struct tuple_adaptor_closure {
        private:
            template<typename T>
            static constexpr auto is_closure_v_ = std::derived_from<std::remove_cvref_t<T>, tuple_adaptor_closure>;
        public:
            template<typename Self, typename Tup>
            requires is_closure_v_<Self> && tuple_adaptor_invocable<Self, Tup>
            friend constexpr auto operator|(Tup &&tup, Self &&self) {
                return std::forward<Self>(self)(std::forward<Tup>(tup));
            }

            template<typename Lhs, typename Rhs>
            requires is_closure_v_<Lhs> && is_closure_v_<Rhs>
            friend constexpr auto operator|(Lhs lhs, Rhs rhs) {
                return pipe<Lhs, Rhs>(std::move(lhs), std::move(rhs));
            }
        };

        template<typename F, typename... Args>
        concept adaptor_partial_app_viable = (F::arity_ > 1)
                                             && (sizeof...(Args) == F::arity_ - 1)
                                             && (std::constructible_from<std::decay_t<Args>, Args> && ...);

        template<typename Derived>
        struct tuple_adaptor {
            template<typename... Args>
            requires adaptor_partial_app_viable<Derived, Args...>
            constexpr auto operator()(Args... args) const {
                return partial<Derived, std::decay_t<Args>...>(std::forward<Args>(args)...);
            }
        };

        template<typename F, typename... Args>
        struct partial : tuple_adaptor_closure {
            std::tuple<Args...> args_;

            constexpr explicit partial(Args... args) : args_(std::move(args...)) {}

            template<typename Tup>
            requires tuple_adaptor_invocable<F, Tup, Args...>
            constexpr auto operator()(Tup &&tup) const &{
                auto forwarder = [&tup](const auto &... args) {
                    return F{}(std::forward<Tup>(tup), args...);
                };

                return std::apply(forwarder, args_);
            }

            template<typename Tup>
            requires tuple_adaptor_invocable<F, Tup, Args...>
            constexpr auto operator()(Tup &&tup) &&{
                auto forwarder = [&tup](auto &... args) {
                    return F{}(std::forward<Tup>(tup), std::move(args)...);
                };

                return std::apply(forwarder, args_);
            }

            template<typename Tup>
            constexpr auto operator()(Tup &&tup) const && = delete;
        };

        template<typename F, typename Arg>
        struct partial<F, Arg> : tuple_adaptor_closure {
            Arg arg;

            constexpr explicit partial(Arg arg) : arg(std::move(arg)) {}

            template<typename Tup>
            requires tuple_adaptor_invocable<F, Tup, Arg>
            constexpr auto operator()(Tup &&tup) const &{
                return F{}(std::forward<Tup>(tup), arg);
            }

            template<typename Tup>
            requires tuple_adaptor_invocable<F, Tup, Arg>
            constexpr auto operator()(Tup &&tup) &&{
                return F{}(std::forward<Tup>(tup), std::move(arg));
            }

            template<typename Tup>
            constexpr auto operator()(Tup &&tup) const && = delete;
        };

        template<typename F>
        concept closure_has_simple_call_op = F::has_simple_call_op_;

        template<typename F, typename... Args>
        concept adaptor_has_simple_extra_args = F::has_simple_extra_args
                                                || F::template has_simple_extra_args<Args...>;

        template<typename F, typename... Args> requires adaptor_has_simple_extra_args<F, Args...>
                                                        && (std::is_trivially_copyable_v<Args> && ...)
        struct partial<F, Args...> : tuple_adaptor_closure {
            std::tuple<Args...> args_;

            constexpr explicit partial(Args... args) : args_(std::move(args)...) {}

            template<typename Tup>
            requires tuple_adaptor_invocable<F, Tup, const Args &...>
            constexpr auto operator()(Tup &&tup) const {
                auto forwarder = [&tup](const auto &... args) {
                    return F{}(std::forward<Tup>(tup), args...);
                };

                return std::apply(forwarder, args_);
            }

            static constexpr bool has_simple_call_op_ = true;
        };

        template<typename F, typename Arg> requires adaptor_has_simple_extra_args<F, Arg>
                                                    && (std::is_trivially_copyable_v<Arg>)
        struct partial<F, Arg> : tuple_adaptor_closure {
            Arg arg_;

            constexpr explicit partial(Arg args) : arg_(std::move(args)) {}

            template<typename Tup>
            requires tuple_adaptor_invocable<F, Tup, const Arg &>
            constexpr auto operator()(Tup &&tup) const {
                auto forwarder = [&tup](const auto &arg) {
                    return F{}(std::forward<Tup>(tup), arg);
                };

                return std::apply(forwarder, arg_);
            }

            static constexpr bool has_simple_call_op_ = true;
        };

        template<typename T1, typename T2, typename Tup>
        concept pipe_invocable = std::declval<T2>(std::declval<T1>()(std::declval<Tup>()));

        template<typename T1, typename T2>
        struct pipe : tuple_adaptor_closure {
            [[no_unique_address]] T1 lhs_;
            [[no_unique_address]] T2 rhs_;

            constexpr pipe(T1 lhs, T2 rhs) : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

            template<typename Tup>
            requires pipe_invocable<const T1 &, const T2 &, Tup>
            constexpr auto operator()(Tup &&tup) const &{
                return rhs_(lhs_(std::forward<Tup>(tup)));
            }

            template<typename Tup>
            requires pipe_invocable<T1, T2, Tup>
            constexpr auto operator()(Tup &&tup) &&{
                return std::move(rhs_)(std::move(lhs_)(std::forward<Tup>(tup)));
            }

            template<typename Tup>
            constexpr auto operator()(Tup &&tup) const && = delete;
        };

        template<typename T1, typename T2> requires closure_has_simple_call_op<T1> && closure_has_simple_call_op<T2>
        struct pipe<T1, T2> : tuple_adaptor_closure {
            [[no_unique_address]] T1 lhs_;
            [[no_unique_address]] T2 rhs_;

            constexpr pipe(T1 lhs, T2 rhs) : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

            template<typename Tup>
            requires pipe_invocable<const T1 &, const T2 &, Tup>
            constexpr auto operator()(Tup &&tup) const {
                return rhs_(lhs_(std::forward<Tup>(tup)));
            }

            static constexpr bool has_simple_call_op_ = true;
        };
    }

    template<typename Derived> requires std::is_class_v<Derived> && std::same_as<Derived, std::remove_cv_t<Derived>>
    class tuple_adaptor_closure : public views::tuple::_adaptor::tuple_adaptor_closure {

    };

    using views::tuple::_adaptor::tuple_adaptor;

}

#endif //TUPLE_VIEWS_TUPLE_ADAPTOR_HXX
