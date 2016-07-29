
/* 
 * Stores: ID, name, head
 * Primary Key: ID 
 * Reference Key: null
 */
CREATE TABLE Departments (
	DepartmentID INTEGER NOT NULL,
	DepartmentName VARCHAR(50) NULL,
	DepartmentHead VARCHAR(50) NULL,
	PRIMARY KEY (DepartmentID)
);
/* 
 * Stores: ID, name
 * Primary Key: ID 
 * Reference Key: DepartmentID
 */
CREATE TABLE Faculties (
	FacultyID INTEGER NOT NULL, 
	FacultyName VARCHAR(50) NULL,
	DepartmentID INTEGER NULL REFERENCES Departments(DepartmentID),
	PRIMARY KEY (FacultyID)
);
/* 
 * Stores: ID, name, semester, year, meetAt, room
 * Primary Key: ID 
 * Reference Key: FacultyID
 */
CREATE TABLE Courses (
	CourseID INTEGER NOT NULL,
	CourseName VARCHAR(50) NULL,
	Semester VARCHAR(50) NULL,
	Year INTEGER NULL,
	MeetAt VARCHAR(50) NULL,
	Room VARCHAR(50) NULL,
	FacultyID INTEGER NULL REFERENCES Faculties(FacultyID),
	PRIMARY KEY (CourseID)
);
/* 
 * Stores: ID, name
 * Primary Key: ID 
 * Reference Key: null
 */
 CREATE TABLE Students (
 	StudentID INTEGER NOT NULL,
 	StudentName VARCHAR(50) NULL,
 	PRIMARY KEY (StudentID)
 );
/* 
 * Stores: null
 * Primary Key: CourseID, StudentID
 * Reference Key: CourseID, StudentID
 */
CREATE TABLE Enrollment (
	CourseID INTEGER NOT NULL REFERENCES Courses(CourseID),
	StudentID INTEGER NOT NULL REFERENCES Students(StudentID),
	PRIMARY KEY(CourseID, StudentID)
);
/* 
 * Stores: ID, type, wreight, deadline, meetngroom
 * Primary Key: EvaluationID
 * Reference Key: CourseID
 */
CREATE TABLE Evaluations (
	EvaluationID INTEGER NOT NULL,
	EvaluationType VARCHAR(50) NULL,
	EvaluationWeight DECIMAL(2,2) NULL, 
	EvaluationDeadline VARCHAR(50) NULL,
	EvaluationMeetingRoom VARCHAR(50) NULL,
	CourseID INTEGER NOT NULL REFERENCES Courses(CourseID),
	PRIMARY KEY(EvaluationID)
);
/* 
 * Stores: grade
 * Primary Key: EvaluationID
 * Reference Key: CourseID
 */
 CREATE TABLE Grades (
 	Grade DECIMAL(5,2) NULL,
	StudentID INTEGER NOT NULL REFERENCES Students(StudentID),
 	EvaluationID INTEGER NOT NULL REFERENCES Evaluations(EvaluationID),
	PRIMARY KEY(StudentID, EvaluationID)
 ); 


