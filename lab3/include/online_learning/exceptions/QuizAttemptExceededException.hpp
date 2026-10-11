#pragma once
#include "online_learning/exceptions/PlatformException.hpp"

namespace online_learning {

class QuizAttemptExceededException : public PlatformException {
public:
    explicit QuizAttemptExceededException(const std::string& quizId, int maxAttempts)
        : PlatformException("Maximum attempt limit (" + std::to_string(maxAttempts) +
                            ") reached for quiz: " + quizId) {}
};

} // namespace online_learning
