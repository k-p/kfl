#pragma once

#include <memory>
#include <stdexcept>

namespace kfl {

    template <typename T>
    class List {
    private:

        class Cell final {
        public:
            inline Cell(const T& head, std::shared_ptr<Cell> tail) : m_head{head}, m_tail{tail} {}
            inline ~Cell() = default;

            inline const T& head() const { return m_head; }
            inline std::shared_ptr<Cell> tail() const { return m_tail; }
            inline bool operator==(const Cell& rhs) const {
                return (m_head == rhs.m_head)
                    && ((m_tail == rhs.m_tail) || (m_tail && rhs.m_tail && *m_tail == *rhs.m_tail));
            }

        private:
            const T m_head;
            const std::shared_ptr<Cell> m_tail;
        };

    public:
        List() = default;
        List(const List&) = default;
        List& operator=(List&) = default;
        virtual ~List() = default;

        List(List&&) = delete;
        List& operator=(List&&) = delete;

        bool empty() const {
            return !m_cell;
        }

        List cons(const T& value) const {
            return std::make_shared<Cell>(value, m_cell);
        }

        const T& head() const {
            return (m_cell ? m_cell->head() : throw std::runtime_error{"Cannot take head() of an empty list."});
        }

        List tail() const {
            return (m_cell ? m_cell->tail() : throw std::runtime_error{"Cannot take tail() of an empty list."});
        }

        bool operator==(const List& rhs) const {
            return (m_cell == rhs.m_cell)
                || (m_cell && rhs.m_cell && *m_cell == *rhs.m_cell);
        }

    private:
        List(std::shared_ptr<Cell> cell) : m_cell{cell} {}
        const std::shared_ptr<Cell> m_cell = nullptr;
    };

}