// Написать алгоритм для решения игры в “пятнашки”.
// Решением задачи является приведение к виду : [1 2 3 4][5 6 7 8][9 10 11 12][13 14 15 0],
// где 0 задает пустую ячейку.Достаточно найти хотя бы какое - то решение.
// Число перемещений костяшек не обязано быть минимальным.

// Пробовал все эвристики и не проходили тесты, но потом решил удалять плохие решения и заработало)
#include <iostream>
#include <cstring>
#include <cassert>
#include <array>
#include <unordered_map>
#include <queue>
#include <set>
#include <algorithm>

const char FieldSize = 16;
const std::array<char, FieldSize> finishField = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};

class GameState
{
public:
    GameState(const std::array<char, FieldSize> &field) : field(field)
    {
        emptyPos = -1;
        for (int i = 0; i < FieldSize; i++)
        {
            if (field[i] == 0)
            {
                emptyPos = i;
                break;
            }
        }
    }

    GameState(const GameState &other) : field(other.field), emptyPos(other.emptyPos) {}

    bool IsComplete() const
    {
        return field == finishField;
    }

    bool IsSolvable() const
    {
        int row = -1;
        for (int i = 0; i < FieldSize; i++)
        {
            if (field[i] == 0)
            {
                row = i / 4 + 1;
            }
        }
        return (getInvCount() + row) % 2 == 0;
    }

    bool CanMoveLeft() const
    {
        return emptyPos % 4 != 3;
    }

    bool CanMoveRight() const
    {
        return emptyPos % 4 != 0;
    }

    bool CanMoveUp() const
    {
        return emptyPos < 12;
    }

    bool CanMoveDown() const
    {
        return emptyPos > 3;
    }

    int Heuristic() const
    {
        int h = 0;
        for (int i = 0; i < FieldSize; i++)
        {
            if (field[i] == 0)
                continue;
            int target = field[i] - 1;
            h += std::abs(i / 4 - target / 4) + abs(i % 4 - target % 4);
        }

        for (int row = 0; row < 4; ++row)
        {
            for (int i = 0; i < 4; ++i)
            {
                int indexI = row * 4 + i;
                int tileI = field[indexI];
                if (tileI == 0 || (tileI - 1) / 4 != row)
                    continue;

                for (int j = i + 1; j < 4; ++j)
                {
                    int indexJ = row * 4 + j;
                    int tileJ = field[indexJ];
                    if (tileJ == 0 || (tileJ - 1) / 4 != row)
                        continue;

                    if (tileI > tileJ)
                    {
                        h += 2;
                    }
                }
            }
        }

        for (int col = 0; col < 4; ++col)
        {
            for (int i = 0; i < 4; ++i)
            {
                int indexI = i * 4 + col;
                int tileI = field[indexI];
                if (tileI == 0 || (tileI - 1) % 4 != col)
                    continue;

                for (int j = i + 1; j < 4; ++j)
                {
                    int indexJ = j * 4 + col;
                    int tileJ = field[indexJ];
                    if (tileJ == 0 || (tileJ - 1) % 4 != col)
                        continue;

                    if (tileI > tileJ)
                    {
                        h += 2;
                    }
                }
            }
        }

        return h;
    }

    GameState MoveLeft() const
    {
        assert(CanMoveLeft());

        GameState newState(*this);
        std::swap(newState.field[emptyPos], newState.field[emptyPos + 1]);
        newState.emptyPos++;
        return newState;
    }

    GameState MoveRight() const
    {
        assert(CanMoveRight());

        GameState newState(*this);
        std::swap(newState.field[emptyPos], newState.field[emptyPos - 1]);
        newState.emptyPos--;
        return newState;
    }

    GameState MoveUp() const
    {
        assert(CanMoveUp());

        GameState newState(*this);
        std::swap(newState.field[emptyPos], newState.field[emptyPos + 4]);
        newState.emptyPos += 4;
        return newState;
    }

    GameState MoveDown() const
    {
        assert(CanMoveDown());

        GameState newState(*this);
        std::swap(newState.field[emptyPos], newState.field[emptyPos - 4]);
        newState.emptyPos -= 4;
        return newState;
    }

    bool operator==(const GameState &other) const
    {
        return field == other.field;
    }

    bool operator<(const GameState &other) const
    {
        return field < other.field;
    }

    friend struct GameStateHash;
    friend std::ostream &operator<<(std::ostream &, const GameState &);

private:
    size_t getInvCount() const
    {
        size_t inv_count = 0;
        for (int i = 0; i < FieldSize - 1; i++)
        {
            for (int j = i + 1; j < FieldSize; j++)
            {
                if (field[i] > field[j] && field[i] && field[j])
                    inv_count++;
            }
        }
        return inv_count;
    }

    std::array<char, FieldSize> field;
    char emptyPos;
};

std::ostream &operator<<(std::ostream &out, const GameState &state)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            out << static_cast<int>(state.field[i * 4 + j]) << " ";
        }
        out << std::endl;
    }
    return out;
}

struct GameStateHash
{
public:
    size_t operator()(const GameState &state) const
    {
        size_t hash = 0;
        std::memcpy(&hash, state.field.data(), sizeof(hash));
        return hash;
    }
};

std::string GetSolution(const std::array<char, FieldSize> &field)
{
    GameState startState(field);

    if (!startState.IsSolvable())
    {
        return "-1";
    }

    std::unordered_map<GameState, char, GameStateHash> visited;
    visited[startState] = 'S';

    std::set<std::pair<int, GameState>> queue;
    queue.insert({0, startState});

    int maxQ = 5000;

    while (true)
    {
        auto [curDist, state] = *queue.begin();
        queue.erase(queue.begin());

        if (state.IsComplete())
        {
            break;
        }

        int dist = curDist + 1;

        if (state.CanMoveLeft())
        {
            GameState newState = state.MoveLeft();
            if (visited.find(newState) == visited.end())
            {
                visited[newState] = 'L';
                queue.insert({dist + newState.Heuristic(), newState});
            }
        }

        if (state.CanMoveRight())
        {
            GameState newState = state.MoveRight();
            if (visited.find(newState) == visited.end())
            {
                visited[newState] = 'R';
                queue.insert({dist + newState.Heuristic(), newState});
            }
        }

        if (state.CanMoveDown())
        {
            GameState newState = state.MoveDown();
            if (visited.find(newState) == visited.end())
            {
                visited[newState] = 'D';
                queue.insert({dist + newState.Heuristic(), newState});
            }
        }

        if (state.CanMoveUp())
        {
            GameState newState = state.MoveUp();
            if (visited.find(newState) == visited.end())
            {
                visited[newState] = 'U';
                queue.insert({dist + newState.Heuristic(), newState});
            }
        }

        while (queue.size() > maxQ)
        {
            auto it = queue.end();
            it--;
            queue.erase(it);
        }
    }

    std::string path;
    GameState state(finishField);

    while (visited[state] != 'S')
    {
        char move = visited[state];
        switch (move)
        {
        case 'L':
        {
            state = state.MoveRight();
            path += 'L';
            break;
        }
        case 'R':
        {
            state = state.MoveLeft();
            path += 'R';
            break;
        }
        case 'U':
        {
            state = state.MoveDown();
            path += 'U';
            break;
        }
        case 'D':
        {
            state = state.MoveUp();
            path += 'D';
            break;
        }
        }
    }
    std::reverse(path.begin(), path.end());
    return path;
}

int main()
{
    std::array<char, FieldSize> field;
    for (int i = 0; i < FieldSize; ++i)
    {
        int temp;
        std::cin >> temp;
        field[i] = static_cast<char>(temp);
    }
    std::string res = GetSolution(field);
    if (res != "-1")
    {
        std::cout << res.size() << '\n';
    }
    std::cout << res;
    return 0;
}