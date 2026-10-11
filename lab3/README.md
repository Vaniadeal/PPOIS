# Lab3 — Платформа онлайн-обучения (Online Learning Platform)

Крупный ООП-проект на C++17. Предметная область — комплексная платформа дистанционного образования:
каталог курсов, модули и интерактивные уроки, система тестирования и домашних заданий, аналитика успеваемости,
форумы и поддержка, платежи, подписки, промокоды и выдача верифицируемых сертификатов.

---

## 1. Соответствие требованиям

| Показатель | Требуется по заданию | Реализовано в проекте |
|---|---|---|
| **Классы (всего)** | ≥ 50 | **71** (57 доменных + 14 классов исключений) |
| **Поля** | ≥ 150 | **260+** |
| **Уникальные поведения (методы)** | ≥ 100 | **160+** |
| **Ассоциации между классами** | ≥ 30 | **55** |
| **Собственные классы исключений** | ≥ 12 | **14** |

---

## 2. Архитектура и структура классов

### Подсистема 1: Пользователи и авторизация (Users & Auth)
1. `User` — базовый класс пользователя (ID, email, имя, статус активности).
2. `Student` — студент платформы (баланс кошелька, список зачислений, уровень).
3. `Instructor` — преподаватель (квалификация, рейтинг, опубликованные курсы, доход).
4. `TeachingAssistant` — ассистент курса (назначенные курсы, права проверки работ).
5. `Administrator` — администратор платформы (уровень доступа, отдел, аудит).
6. `UserProfile` — профиль пользователя (био, аватар, телефон, часовой пояс, страна).
7. `UserCredentials` — безопасность учетных записей (логин, хеш пароля, соль, попытки входа).
8. `RoleManager` — управление ролями и правами доступа.
9. `Session` — пользовательская веб-сессия (токен, IP, время жизни, валидация).
10. `AuthenticationService` — сервис аутентификации, регистрации и блокировки аккаунтов.

### Подсистема 2: Курсы и учебный контент (Courses & Content)
11. `Course` — курс (название, описание, уровень сложности, цена, статус публикации).
12. `CourseModule` — учебный модуль внутри курса (порядковый номер, проходной балл).
13. `Lesson` — базовый класс урока (длительность, порядок, бесплатное превью).
14. `VideoLesson` — видеоурок (URL видео, разрешение, субтитры, длительность в секундах).
15. `TextArticleLesson` — текстовый урок (текст статьи, расчетное время чтения, счетчик слов).
16. `InteractiveCodingLesson` — интерактивная задача по программированию (язык, стартовый код, тесты).
17. `Attachment` — дополнительный файл к уроку (размер, тип, ссылка).
18. `CourseSyllabus` — учебный план (цели обучения, пререквизиты, суммарная длительность).
19. `CourseCategory` — категория курсов (название, slug, родительская категория).
20. `Tag` — тег курса (название, счетчик использований).

### Подсистема 3: Тестирование и домашние задания (Assessments & Homework)
21. `Quiz` — тест по модулю (лимит времени, макс. попыток, проходной процент).
22. `Question` — базовый класс вопроса (текст, баллы, тип).
23. `SingleChoiceQuestion` — вопрос с одиночным выбором (варианты, правильный индекс).
24. `MultipleChoiceQuestion` — вопрос с множественным выбором (битовая маска/набор правильных).
25. `CodingQuestion` — автоматизированная задача (тестовые кейсы, шаблон решения).
26. `QuizAttempt` — попытка прохождения теста студентом (таймстампы, набранный балл, статус сдачи).
27. `StudentAnswer` — ответ студента на конкретный вопрос.
28. `HomeworkAssignment` — домашнее задание (дедлайн, формулировка, максимальный балл).
29. `HomeworkSubmission` — решение студента (текст решения, ссылка на архив, время отправки).
30. `HomeworkReview` — рецензия преподавателя (оценка, текстовый фидбек, дата проверки).

### Подсистема 4: Прогресс, успеваемость и аналитика (Progress & Analytics)
31. `Enrollment` — запись студента на курс (прогресс в %, дата завершения, статус).
32. `LessonProgress` — статус прохождения отдельного урока (время просмотра, флаг завершения).
33. `GradeBook` — ведомость курса (шкала оценивания, минимальный порог).
34. `StudentGrade` — итоговая оценка студента по курсу (баллы, буквенная оценка, зачет).
35. `LearningStreakTracker` — трекер ежедневной активности (текущий стрик, рекордный стрик).
36. `CourseAnalytics` — сводная статистика курса (число студентов, средний рейтинг, конверсия).
37. `EngagementMetric` — метрики вовлеченности (время на уроке, точка оттока студентов).

### Подсистема 5: Сообщество, отзывы и поддержка (Community & Support)
38. `CourseReview` — отзыв и оценка курса студентом (рейтинг 1-5, верификация покупки).
39. `DiscussionForum` — форум обсуждений при курсе.
40. `ForumThread` — тема обсуждения на форуме (автор, статус закрепления/закрытия).
41. `ForumPost` — сообщение в ветке обсуждения (голоса за полезность, флаг решения).
42. `SupportTicket` — тикет в службу поддержки (приоритет, статус, назначенный админ).
43. `TicketResponse` — ответ на тикет поддержки.

### Подсистема 6: Финансы, подписки и промокоды (Finance & Monetization)
44. `PaymentTransaction` — транзакция оплаты (сумма, валюта, статус, метод оплаты).
45. `Order` — заказ на покупку курса (базовая цена, скидка, итоговая сумма, статус).
46. `PromoCode` — промокод на скидку (процент скидки, срок действия, лимит активаций).
47. `SubscriptionPlan` — тарифный план подписки (ежемесячная цена, лимит курсов, уровень).
48. `UserSubscription` — активная подписка пользователя (период, автопродление).
49. `RefundRequest` — заявка на возврат средств (причина, одобрение, сумма возврата).
50. `PayoutRecord` — выплата гонорара преподавателю (комиссия платформы, статус выплаты).

### Подсистема 7: Сертификация и достижения (Certification & Gamification)
51. `Certificate` — именной сертификат об окончании курса (уникальный код верификации).
52. `CertificateTemplate` — шаблон оформления сертификата (стиль рамки, подписи).
53. `AchievementBadge` — наградной бейдж за успехи (критерий получения, иконка).
54. `StudentBadgeAward` — факт вручения бейджа студенту (дата награждения).

### Подсистема 8: Системные сервисы и Фасад (Platform Services & Facade)
55. `Notification` — системное уведомление пользователю (заголовок, текст, статус прочтения).
56. `NotificationService` — сервис диспетчеризации уведомлений.
57. `OnlineLearningPlatform` — центральный фасад системы, координирующий все подсистемы.

### Подсистема 9: Собственные исключения (Custom Exceptions)
58. `PlatformException` — базовое исключение платформы (унаследовано от `std::runtime_error`).
59. `UserNotFoundException` — пользователь с указанным ID/email не найден.
60. `AuthenticationFailedException` — неверный логин или пароль, превышение попыток входа.
61. `UserAlreadyExistsException` — попытка зарегистрировать дублирующий email.
62. `CourseNotFoundException` — курс с указанным ID не существует.
63. `CourseAlreadyPublishedException` — попытка изменить уже опубликованный активный курс.
64. `EnrollmentNotFoundException` — запись студента на курс не найдена.
65. `AlreadyEnrolledException` — студент уже зачислен на этот курс.
66. `QuizAttemptExceededException` — исчерпано максимальное количество попыток теста.
67. `DeadlinePassedException` — дедлайн сдачи домашнего задания истек.
68. `InvalidPromoCodeException` — промокод недействителен, просрочен или исчерпан.
69. `InsufficientFundsException` — недостаточно средств на балансе студента для оплаты.
70. `CertificateAlreadyIssuedException` — сертификат по данному курсу уже был выдан ранее.
71. `AccessDeniedException` — недостаточно прав для выполнения операции (ролевой доступ).

---

## 3. Сводная таблица классов

| Класс | Поля | Методы | Ассоциации (связанные классы) |
|---|:---:|:---:|---|
| `User` | 6 | 4 | `UserProfile` |
| `Student` | 5 | 5 | `User`, `Enrollment`, `Certificate`, `StudentGrade` |
| `Instructor` | 5 | 5 | `User`, `Course`, `PayoutRecord` |
| `TeachingAssistant` | 4 | 4 | `User`, `Course`, `HomeworkReview` |
| `Administrator` | 4 | 4 | `User`, `SupportTicket` |
| `UserProfile` | 6 | 4 | `User` |
| `UserCredentials` | 5 | 4 | `User` |
| `RoleManager` | 2 | 4 | `User`, `AccessDeniedException` |
| `Session` | 6 | 4 | `User` |
| `AuthenticationService` | 3 | 5 | `User`, `UserCredentials`, `Session`, `AuthenticationFailedException` |
| `Course` | 8 | 7 | `Instructor`, `CourseModule`, `CourseSyllabus`, `CourseCategory`, `Tag` |
| `CourseModule` | 6 | 5 | `Course`, `Lesson`, `Quiz` |
| `Lesson` | 6 | 4 | `CourseModule`, `Attachment` |
| `VideoLesson` | 4 | 4 | `Lesson` |
| `TextArticleLesson` | 3 | 4 | `Lesson` |
| `InteractiveCodingLesson` | 4 | 4 | `Lesson` |
| `Attachment` | 5 | 3 | `Lesson` |
| `CourseSyllabus` | 4 | 4 | `Course` |
| `CourseCategory` | 5 | 4 | `Course` |
| `Tag` | 3 | 3 | `Course` |
| `Quiz` | 6 | 5 | `CourseModule`, `Question`, `QuizAttempt`, `QuizAttemptExceededException` |
| `Question` | 5 | 3 | `Quiz` |
| `SingleChoiceQuestion` | 3 | 3 | `Question` |
| `MultipleChoiceQuestion` | 3 | 3 | `Question` |
| `CodingQuestion` | 3 | 3 | `Question` |
| `QuizAttempt` | 7 | 5 | `Quiz`, `Student`, `StudentAnswer` |
| `StudentAnswer` | 6 | 4 | `QuizAttempt`, `Question` |
| `HomeworkAssignment` | 6 | 4 | `Lesson`, `HomeworkSubmission`, `DeadlinePassedException` |
| `HomeworkSubmission` | 6 | 4 | `HomeworkAssignment`, `Student`, `HomeworkReview` |
| `HomeworkReview` | 6 | 4 | `HomeworkSubmission`, `Instructor` |
| `Enrollment` | 7 | 6 | `Student`, `Course`, `LessonProgress` |
| `LessonProgress` | 6 | 4 | `Enrollment`, `Lesson` |
| `GradeBook` | 5 | 5 | `Course`, `StudentGrade` |
| `StudentGrade` | 5 | 4 | `GradeBook`, `Student` |
| `LearningStreakTracker` | 4 | 4 | `Student` |
| `CourseAnalytics` | 6 | 5 | `Course` |
| `EngagementMetric` | 5 | 4 | `Course`, `Lesson` |
| `CourseReview` | 7 | 4 | `Course`, `Student` |
| `DiscussionForum` | 5 | 4 | `Course`, `ForumThread` |
| `ForumThread` | 7 | 5 | `DiscussionForum`, `User`, `ForumPost` |
| `ForumPost` | 6 | 4 | `ForumThread`, `User` |
| `SupportTicket` | 7 | 5 | `User`, `Administrator`, `TicketResponse` |
| `TicketResponse` | 5 | 3 | `SupportTicket`, `User` |
| `PaymentTransaction` | 7 | 4 | `User`, `Order`, `InsufficientFundsException` |
| `Order` | 7 | 5 | `Student`, `Course`, `PromoCode`, `PaymentTransaction` |
| `PromoCode` | 6 | 4 | `Order`, `InvalidPromoCodeException` |
| `SubscriptionPlan` | 6 | 4 | `UserSubscription` |
| `UserSubscription` | 6 | 5 | `Student`, `SubscriptionPlan` |
| `RefundRequest` | 6 | 4 | `Order`, `PaymentTransaction` |
| `PayoutRecord` | 6 | 4 | `Instructor`, `PaymentTransaction` |
| `Certificate` | 6 | 4 | `Student`, `Course`, `CertificateTemplate`, `CertificateAlreadyIssuedException` |
| `CertificateTemplate` | 5 | 3 | `Certificate` |
| `AchievementBadge` | 5 | 3 | `StudentBadgeAward` |
| `StudentBadgeAward` | 4 | 3 | `Student`, `AchievementBadge` |
| `Notification` | 6 | 4 | `User` |
| `NotificationService` | 2 | 4 | `Notification`, `User` |
| `OnlineLearningPlatform` | 8 | 12 | `Student`, `Instructor`, `Course`, `Order`, `Certificate`, `NotificationService` |
| *Классы исключений (14)* | — | — | `std::runtime_error` |

---

## 4. Сборка и запуск

### Сборка проекта
```bash
cmake -S lab3 -B lab3/build -DCMAKE_BUILD_TYPE=Release
cmake --build lab3/build
```

### Запуск интерактивного консольного приложения (CLI)
```bash
./lab3/build/learning_platform_app
```

### Запуск unit-тестов
```bash
ctest --test-dir lab3/build --output-on-failure
# или прямой запуск бинарника тестов:
./lab3/build/learning_platform_tests
```

### Проверка покрытия кода (требуется ≥ 90%)
```bash
cmake -S lab3 -B lab3/build-cov -G Ninja \
    -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON
cmake --build lab3/build-cov
ctest --test-dir lab3/build-cov
cd lab3 && gcovr --root . --filter "src/.*" --print-summary --fail-under-line 90
```

