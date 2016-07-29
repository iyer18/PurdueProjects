INSERT INTO Departments ( DepartmentID, DepartmentName, DepartmentHead) VALUES ( 1, 'Software Engineering', 'Dunsmore');
INSERT INTO Departments ( DepartmentID, DepartmentName, DepartmentHead) VALUES ( 2, 'Machine Intelligence ', 'Clifton');
INSERT INTO Departments ( DepartmentID, DepartmentName, DepartmentHead) VALUES ( 3, 'Database and Information Systems', 'Aref');

INSERT INTO Faculties (FacultyID, FacultyName, DepartmentID) VALUES (1, 'Dunsmore', 1);
INSERT INTO Faculties (FacultyID, FacultyName, DepartmentID) VALUES (2, 'Zhang', 1);
INSERT INTO Faculties (FacultyID, FacultyName, DepartmentID) VALUES (3, 'Clifton', 2);
INSERT INTO Faculties (FacultyID, FacultyName, DepartmentID) VALUES (4, 'Xu', 2);
INSERT INTO Faculties (FacultyID, FacultyName, DepartmentID) VALUES (5, 'Aref', 3);
INSERT INTO Faculties (FacultyID, FacultyName, DepartmentID) VALUES (6, 'Bertino', 3);
INSERT INTO Faculties (FacultyID, FacultyName, DepartmentID) VALUES (7, 'Hambrusch', 3);
INSERT INTO Faculties (FacultyID, FacultyName, DepartmentID) VALUES (8, 'Aliaga', 3);

INSERT INTO Students (StudentID, StudentName) VALUES (1, 'Raj I');
INSERT INTO Students (StudentID, StudentName) VALUES (2, 'Robyn B');
INSERT INTO Students (StudentID, StudentName) VALUES (3, 'Yash S');
INSERT INTO Students (StudentID, StudentName) VALUES (4, 'Eehita P');
INSERT INTO Students (StudentID, StudentName) VALUES (5, 'Janka G');
INSERT INTO Students (StudentID, StudentName) VALUES (6, 'Joyce K');

INSERT INTO Courses (CourseID, CourseName, Semester, Year, MeetAt, Room, FacultyID) VALUES (1, 'Information Systems', 'Spring', 2016, '9:30 AM', 'LWSN B155', 6);
INSERT INTO Courses (CourseID, CourseName, Semester, Year, MeetAt, Room, FacultyID) VALUES (2, 'Software Engineering', 'Spring', 2016, '1:30 PM', 'WTHR 172', 1);
INSERT INTO Courses (CourseID, CourseName, Semester, Year, MeetAt, Room, FacultyID) VALUES (3, 'Algorithms', 'Spring', 2016, '4:30 PM', 'LYNN 1136', 8);

INSERT INTO Evaluations (EvaluationID, EvaluationType, EvaluationWeight, EvaluationDeadline, EvaluationMeetingRoom, CourseID) VALUES (1, 'Project', 0.10, '11-MAR-16', 'LWSN B155', 1);
INSERT INTO Evaluations (EvaluationID, EvaluationType, EvaluationWeight, EvaluationDeadline, EvaluationMeetingRoom, CourseID) VALUES (2, 'Midterm', 0.25, '8-MAR-16', 'MATH 175', 1);
INSERT INTO Evaluations (EvaluationID, EvaluationType, EvaluationWeight, EvaluationDeadline, EvaluationMeetingRoom, CourseID) VALUES (3, 'Midterm', 0.25, '24-FEB-16', 'WTHR 173', 2);
INSERT INTO Evaluations (EvaluationID, EvaluationType, EvaluationWeight, EvaluationDeadline, EvaluationMeetingRoom, CourseID) VALUES (4, 'Midterm', 0.25, '9-MAR-16', 'LYNN 1136', 3);

INSERT INTO Enrollment (CourseID, StudentID) VALUES (1, 1);
INSERT INTO Enrollment (CourseID, StudentID) VALUES (2, 1);
INSERT INTO Enrollment (CourseID, StudentID) VALUES (2, 3);
INSERT INTO Enrollment (CourseID, StudentID) VALUES (3, 1);
INSERT INTO Enrollment (CourseID, StudentID) VALUES (1, 2);
INSERT INTO Enrollment (CourseID, StudentID) VALUES (1, 3);
INSERT INTO Enrollment (CourseID, StudentID) VALUES (3, 4);

INSERT INTO Grades (Grade, StudentID, EvaluationID) VALUES (100.00, 1, 1);
INSERT INTO Grades (Grade, StudentID, EvaluationID) VALUES (80.00, 2, 2);
INSERT INTO Grades (Grade, StudentID, EvaluationID) VALUES (90.00, 1, 3);
INSERT INTO Grades (Grade, StudentID, EvaluationID) VALUES (98.00, 1, 4);






