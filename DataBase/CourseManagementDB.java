import java.util.*;
import java.sql.*;

public class CourseManagementDB {

	Connection con;
	static Scanner in = new Scanner(System.in);

	public CourseManagementDB () {
		try {
			Class.forName( "oracle.jdbc.driver.OracleDriver");
		} catch ( ClassNotFoundException e) {
			e.printStackTrace();
		}
		try { 
			con = DriverManager.getConnection( "jdbc:oracle:thin:@claros.cs.purdue.edu:1524:strep", "iyer18", "qbhDVLmg" );
		} catch ( SQLException e ) {
			System.err.println("Unable to login correctly.");
			e.printStackTrace();
		}
	}
	/* Update: Insert user input into database */
	public void addNewCourse(int facultyId) {
		try {

			System.out.print("Enter Course id:");
			int id = in.nextInt();
			in.nextLine();
			System.out.print("Enter Course name:");
			String name = in.nextLine();
			System.out.print("Enter Course semester:");
			String semester = in.nextLine();
			System.out.print("Enter Course year:");
			int year = in.nextInt();
			in.nextLine();
			System.out.print("Enter Course meet at time:");
			String meet_at = in.nextLine();
			System.out.print("Enter Course room:");
			String room = in.nextLine();

			String insertDepartment = "insert into Courses " + " (CourseID, CourseName, Semester, Year, MeetAt, Room, FacultyID)" + " values (?, ?, ?, ?, ?, ?, ?)";
			PreparedStatement stmt = con.prepareStatement(insertDepartment);
			stmt.setInt(1, id);
			stmt.setString(2, name);
			stmt.setString(3, semester);
			stmt.setInt(4, year);
			stmt.setString(5, meet_at);
			stmt.setString(6, room);
			stmt.setInt(7, facultyId);
			stmt.executeUpdate();
			stmt.close();
		} catch (SQLIntegrityConstraintViolationException e) {
			System.err.println("Another course with this Id already exists");
		}
		catch(SQLException e){
			e.printStackTrace();
		}
	}
	public void updateCourse(int facultyId) {
		try {
			viewCourseByID(facultyId);
			System.out.println("Which course would you like to update? Please Enter CourseID");
			int courseId = in.nextInt();
			in.nextLine();
			String check = "SELECT * FROM Courses WHERE FacultyID=? AND CourseID=?";
			PreparedStatement pst = con.prepareStatement(check);
			pst.setInt(1, facultyId);
			pst.setInt(2, courseId);
			ResultSet rs1 = pst.executeQuery();
			if( rs1.next() != false) {
				String query = "SELECT * FROM Courses WHERE FacultyID=? AND CourseID=?";                        
				PreparedStatement ps = con.prepareStatement(query);
				ps.setInt(1, facultyId);
				ps.setInt(2, courseId);
				ResultSet rs = ps.executeQuery();
				ResultSetMetaData rsmd = rs.getMetaData();
				int columnsNumber = rsmd.getColumnCount();
				while ( rs.next()) {
					System.out.println("CourseID, CourseName, Semester, Year, MeetAt, Room, FacultyID");
					for(int i = 1; i <= columnsNumber; i++)
						System.out.print(rs.getString(i) + " ");
					System.out.println();
				}
				int option = 0;
				while ( option != 6){
					System.out.println( "What would you like to change? \n1.Course Name,\n2.Semester\n3.Year\n4.Meet at time\n5.Room\n6.Quit");
					option = in.nextInt(); 
					in.nextLine();
					String updateQuery = "UPDATE Courses ";
					PreparedStatement ps1;
					switch(option) {
						case 1:
							System.out.println( "What would you like the new course name to be?");
							String cName = in.nextLine();
							updateQuery += "SET CourseName=? " + "WHERE CourseID=?";
							ps1 = con.prepareStatement(updateQuery);
							ps1.setString(1, cName);
							ps1.setInt(2, courseId);
							ps1.executeQuery();
							break;
						case 2:
							System.out.println( "What would you like the new Semester to be?");
							String cSem = in.nextLine();
							updateQuery += "SET Semester=? " + "WHERE CourseID=?";
							ps1 = con.prepareStatement(updateQuery);
							ps1.setString(1, cSem);
							ps1.setInt(2, courseId);
							ps1.executeQuery();
							break;
						case 3:
							System.out.println( "What would you like the new Year to be?");
							int cYear = in.nextInt();
							in.nextLine();
							updateQuery += "SET Year=? " + "WHERE CourseID=?";
							ps1 = con.prepareStatement(updateQuery);
							ps1.setInt(1, cYear);
							ps1.setInt(2, courseId);
							ps1.executeQuery();
							break;
						case 4:
							System.out.println( "What would you like the new Meet at time to be?");
							String cTime = in.nextLine();
							updateQuery += "SET MeetAt=? " + "WHERE CourseID=?";
							ps1 = con.prepareStatement(updateQuery);
							ps1.setString(1, cTime);
							ps1.setInt(2, courseId);
							ps1.executeQuery();
							break;
						case 5:
							System.out.println( "What would you like the new Room to be?");
							String cRoom = in.nextLine();
							updateQuery += "SET Room=? " + "WHERE CourseID=?";
							ps1 = con.prepareStatement(updateQuery);
							ps1.setString(1, cRoom);
							ps1.setInt(2, courseId);
							ps1.executeQuery();
							break;
						case 6:
							break;
						default:
							System.out.println("Wrong Input");
							break;
					}
				}
			}
			else System.err.println("You have no such course");
		} catch (SQLIntegrityConstraintViolationException e) {
			System.err.println("Another course with this Id already exists");		         
		} catch ( SQLException e)  {
			e.printStackTrace();
		}

	}
	public void viewCourse(int facultyId) {
		try {
			String query = "select * from Courses where FacultyID=?";                        
			PreparedStatement ps = con.prepareStatement(query);
			ps.setInt(1, facultyId);
			ResultSet rs = ps.executeQuery();
			while ( rs.next()) 
				System.out.println(rs.getString("CourseName"));
			if( rs == null) 
				System.out.println("You have not signed up for any courses");
		} catch ( SQLException e)  {
			e.printStackTrace();
		}
	}
	public void viewCourseByID(int facultyId) {
		try {
			System.out.println("COURSE LIST:");
			String query = "select * from Courses where FacultyID=?";                        
			PreparedStatement ps = con.prepareStatement(query);
			ps.setInt(1, facultyId);
			ResultSet rs = ps.executeQuery();
			ResultSetMetaData rsmd = rs.getMetaData();
			int columnsNumber = rsmd.getColumnCount();
			if(rs.next() == false)
				System.out.println("You have not signed up for any courses");
			else {
				System.out.println("CourseID, CourseName, Semester, Year, MeetAt, Room, FacultyID");
				for(int i = 1; i <= columnsNumber; i++)
					System.out.print(rs.getString(i) + " \t|");
				System.out.println();

			}
			while ( rs.next()) {
				for(int i = 1; i <= columnsNumber; i++)
					System.out.print(rs.getString(i) + " \t|");
				System.out.println();
			}
		} catch ( SQLException e)  {
			e.printStackTrace();
		}
	}
	public void deleteCourse(int facultyId) {
		try {
			viewCourseByID(facultyId);
			System.out.println("Which course would you like to delete? Please Enter CourseID");
			int courseId = in.nextInt();
			in.nextLine();
			String check = "SELECT * FROM Courses WHERE FacultyID=? AND CourseID=?";
			PreparedStatement ps1 = con.prepareStatement(check);
			ps1.setInt(1, facultyId);
			ps1.setInt(2, courseId);
			ResultSet rs = ps1.executeQuery();
			if( rs.next() != false) {
				String query = "DELETE FROM Courses WHERE FacultyID=? AND CourseID=?";                        
				PreparedStatement ps = con.prepareStatement(query);
				ps.setInt(1, facultyId);
				ps.setInt(2, courseId);
				ps.executeQuery();
				viewCourseByID(facultyId);
				System.err.printf("Course %d has been deleted\n", courseId);
				viewCourseByID(facultyId);
			}
			else System.err.println("You have no such course");
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}


	public void viewStudents() {
		try {
			System.out.println("STUDENT LIST:");
			String query = "SELECT * FROM Students";                        
			PreparedStatement ps = con.prepareStatement(query);
			ResultSet rs = ps.executeQuery();
			ResultSetMetaData rsmd = rs.getMetaData();
			int columnsNumber = rsmd.getColumnCount();
			//System.out.println("colnum:" + columnsNumber);
			if(rs.next() == false)
				System.out.println("There are no Students");
			else {
				System.out.println("StudentID, StudentName");
				for(int i = 1; i <= columnsNumber; i++)
					System.out.print(rs.getString(i) + " \t|");
				System.out.println();

			}
			while ( rs.next()) {
				for(int i = 1; i <= columnsNumber; i++)
					System.out.print(rs.getString(i) + " \t|");
				System.out.println();
			}
		} catch ( SQLException e)  {
			e.printStackTrace();
		}

	}
	public void viewStudentsInCourse(int courseID) {
		try {
			System.out.printf("Students in course %d LIST:\n", courseID);
			String query = "SELECT Enrollment.CourseID, Enrollment.StudentID, Students.StudentName FROM Enrollment, Students WHERE CourseID=? AND Enrollment.StudentID=Students.StudentID";                        
			PreparedStatement ps = con.prepareStatement(query);
			ps.setInt(1, courseID);
			ResultSet rs = ps.executeQuery();
			ResultSetMetaData rsmd = rs.getMetaData();
			int columnsNumber = rsmd.getColumnCount();
			if(rs.next() == false)
				System.out.println("There are no Students in this course");
			else {
				System.out.println("CourseID, StudentID, StudentName");
				for(int i = 1; i <= columnsNumber; i++)
					System.out.print(rs.getString(i) + " \t|");
				System.out.println();

			}
			while ( rs.next()) {
				for(int i = 1; i <= columnsNumber; i++)
					System.out.print(rs.getString(i) + " \t|");
				System.out.println();
			}
		} catch ( SQLException e)  {
			e.printStackTrace();
		}

	}
	public void assignStudent(int facultyId) {
		try {
			System.out.println("For which course would you like to enroll students?");
			viewCourseByID(facultyId);
			int courseID = in.nextInt();
			in.nextLine();
			String check = "SELECT * FROM Courses WHERE FacultyID=? AND CourseID=?";
			PreparedStatement ps1 = con.prepareStatement(check);
			ps1.setInt(1, facultyId);
			ps1.setInt(2, courseID);
			ResultSet rs = ps1.executeQuery();
			if( rs.next() != false) {
				viewStudentsInCourse(courseID);
				System.out.println("Which students would you like to enroll in course " + courseID  + "? Please enter the student id.");
				viewStudents();
				int studentID = in.nextInt();
				in.nextLine();
				if( studentExists(studentID) ) {
					if (studentIsEnrolled(studentID, courseID)) {
						String query = "INSERT INTO Enrollment (CourseID, StudentID) VALUES (?, ?)";
						PreparedStatement ps = con.prepareStatement(query);
						ps.setInt(1, courseID);
						ps.setInt(2, studentID);
						ps.executeQuery();
						viewStudentsInCourse(courseID);
					}
					else System.out.printf("Student %d is already enrolled in course %d\n", studentID, courseID);
				}
				else System.out.println("Student does not exist");
			}
			else System.err.println("You have no such course");
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}
	public boolean studentExists(int studentID) {
		try {
			String check = "SELECT * FROM Students WHERE StudentID=?";
			PreparedStatement ps1 = con.prepareStatement(check);
			ps1.setInt(1, studentID);
			ResultSet rs = ps1.executeQuery();
			if( rs.next() != false)  return true;
			else {
				//System.out.println("Student does not exist");
				return false;
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return false;
	}
	public boolean studentIsEnrolled(int studentID, int courseID) {
		try {
			String check = "SELECT * FROM Enrollment WHERE StudentID=? AND CourseID=?";
			PreparedStatement ps1 = con.prepareStatement(check);
			ps1.setInt(1, studentID);
			ps1.setInt(2, courseID);
			ResultSet rs = ps1.executeQuery();
			if( rs.next() != false)  {
				return true;
			} else 
				return false;
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return false;
	}

	public void addNewEvaluation(int facultyId) { 
		try {
			System.out.println("For which course would you like to add an Evaluation?");
			viewCourseByID(facultyId);
			int courseID = in.nextInt();
			in.nextLine();
			String check = "SELECT * FROM Courses WHERE FacultyID=? AND CourseID=?";
			PreparedStatement ps1 = con.prepareStatement(check);
			ps1.setInt(1, facultyId);
			ps1.setInt(2, courseID);
			ResultSet rs = ps1.executeQuery();
			if( rs.next() != false) {
				viewEvaluationForCourse(courseID);
				System.out.print("Enter Evaluation id:");
				int id = in.nextInt();
				in.nextLine();
				System.out.print("Enter Evaluation Type (HW, Midterm, Final Exam, Project):");
				String type = in.nextLine();
				System.out.print("Enter Evaluation weightage:");
				double weight = in.nextDouble();
				in.nextLine();
				System.out.print("Enter Evaluation deadline(DD-MMM-YY, 11-MAR-16):");
				String due = in.nextLine();
				System.out.print("Enter Evaluation meeting room :");
				String meet_at = in.nextLine();

				String query = "INSERT INTO Evaluations (EvaluationID, EvaluationType, EvaluationWeight, EvaluationDeadline, EvaluationMeetingRoom, CourseID) VALUES (?, ?, ?, ?, ?, ?)";
				PreparedStatement ps = con.prepareStatement(query);
				ps.setInt(1, id);
				ps.setString(2, type);
				ps.setDouble(3, weight);
				ps.setString(4, due);
				ps.setString(5, meet_at);
				ps.setInt(6, courseID);
				ps.executeUpdate();
				//ps.close();
				System.out.println("Evaluation has been added");
				viewEvaluationForCourse(courseID);


			}else System.err.println("You have no such course");
		} catch (SQLIntegrityConstraintViolationException e) {
			System.err.println("Another evaluation with this Id already exists");		         
		} catch (SQLException e) {
			e.printStackTrace();
		}

	}
	public void viewEvaluation(int facultyId) { 
		try{
			System.out.println("For which course would you like to view your Evaluations?");
			viewCourseByID(facultyId);
			int courseID = in.nextInt();
			in.nextLine();
			String check = "SELECT * FROM Courses WHERE FacultyID=? AND CourseID=?";
			PreparedStatement ps1 = con.prepareStatement(check);
			ps1.setInt(1, facultyId);
			ps1.setInt(2, courseID);
			ResultSet rs = ps1.executeQuery();
			if( rs.next() != false) {
				viewEvaluationForCourse(courseID);
			}else System.err.println("You have no such course");
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}
	public void viewEvaluationForCourse(int courseID) { 
		try {
			System.out.printf("Evaluations for course %d LIST:\n", courseID);
			String query = "SELECT * FROM Evaluations WHERE CourseID=?";                        
			PreparedStatement ps = con.prepareStatement(query);
			ps.setInt(1, courseID);
			ResultSet rs = ps.executeQuery();
			ResultSetMetaData rsmd = rs.getMetaData();
			int columnsNumber = rsmd.getColumnCount();
			if(rs.next() == false)
				System.out.println("There are no Evaluations for this course");
			else {
				System.out.println("EvaluationID, EvaluationType, EvaluationWeight, EvaluationDeadline, EvaluationMeetingRoom, CourseID");
				for(int i = 1; i <= columnsNumber; i++)
					System.out.print(rs.getString(i) + " \t|");
				System.out.println();

			}
			while ( rs.next()) {
				for(int i = 1; i <= columnsNumber; i++)
					System.out.print(rs.getString(i) + " \t|");
				System.out.println();
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}

	}
	public boolean isValidCourse(int facultyId, int courseID) {
		try {
			String check = "SELECT * FROM Courses WHERE FacultyID=? AND CourseID=?";
			PreparedStatement ps1 = con.prepareStatement(check);
			ps1.setInt(1, facultyId);
			ps1.setInt(2, courseID);
			ResultSet rs = ps1.executeQuery();
			if( rs.next() != false)  {
				return true;
			} else {
				//System.err.println("You have no such course");
				return false;
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return false;
	}
	public boolean isValidEvaluation(int evalId, int courseID) {
		try {
			String check = "SELECT * FROM Evaluations WHERE EvaluationID=? AND CourseID=?";
			PreparedStatement ps1 = con.prepareStatement(check);
			ps1.setInt(1, evalId);
			ps1.setInt(2, courseID);
			ResultSet rs = ps1.executeQuery();
			if( rs.next() != false)  {
				return true;
			} else {
				//System.err.println("You have no such evaluation");
				return false;
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return false;

	}

	public void updateEvaluation(int facultyId) { 
		try {
			viewCourseByID(facultyId);
			System.out.println( "For which course would you like to update an evaluation, refer above?");
			int courseID = in.nextInt();
			in.nextLine();
			if( isValidCourse(facultyId, courseID)) {
				viewEvaluationForCourse(courseID);
				System.out.println( "Which evaluation would you like to update, refer above?");
				int evalId = in.nextInt();
				in.nextLine();
				if (isValidEvaluation(evalId, courseID)) {
					int option = 0;
					while ( option != 5){
						System.out.println( "What would you like to change? \n1.Type,\n2.Weightage\n3.Deadline\n4.Meet at location\n5.Quit");
						option = in.nextInt(); 
						in.nextLine();
						String updateQuery = "UPDATE Evaluations ";
						PreparedStatement ps1;
						switch(option) {
							case 1:
								System.out.println( "What would you like the new evaluation type to be?");
								String eType = in.nextLine();
								updateQuery += "SET EvauationType=? " + "WHERE EvaluationID=?";
								ps1 = con.prepareStatement(updateQuery);
								ps1.setString(1, eType);
								ps1.setInt(2, evalId);
								ps1.executeQuery();
								break;
							case 2:
								System.out.println( "What would you like the new Weightage to be?");
								double eWeight = in.nextDouble();
								in.nextLine();
								updateQuery += "SET EvaluationWeight=? " + "WHERE EvaluationID=?";
								ps1 = con.prepareStatement(updateQuery);
								ps1.setDouble(1, eWeight);
								ps1.setInt(2, evalId);
								ps1.executeQuery();
								break;
							case 3:
								System.out.println( "What would you like the new Deadline(DD-MMM-YY, 11-MAR-16) to be?");
								String eDue = in.nextLine();
								updateQuery += "SET EvaluationDeadline=? " + "WHERE EvaluationID=?";
								ps1 = con.prepareStatement(updateQuery);
								ps1.setString(1, eDue);
								ps1.setInt(2, evalId);
								ps1.executeQuery();
								break;
							case 4:
								System.out.println( "What would you like the new Meet at location to be?");
								String eLoc = in.nextLine();
								updateQuery += "SET EvaluationMeetingRoom=? " + "WHERE EvaluationID=?";
								ps1 = con.prepareStatement(updateQuery);
								ps1.setString(1, eLoc);
								ps1.setInt(2, evalId);
								ps1.executeQuery();
								break;
							case 5:
								break;
							default:
								System.out.println("Wrong Input");
								break;
						}
					}
				}else System.err.println("You have no such evaluation"); 
			} else System.err.println("You have no such course");
		} catch (SQLIntegrityConstraintViolationException e) {
			System.err.println("Another evaluation with this Id already exists");		         
		} catch (SQLException e) {
			e.printStackTrace();
		}



	}
	public void deleteEvaluation(int facultyId) { 
		try {
			viewCourseByID(facultyId);
			System.out.println("Evaluation in which course would you like to delete? Please Enter CourseID");
			int courseId = in.nextInt();
			in.nextLine();
			if (isValidCourse(facultyId, courseId) ) {
				viewEvaluationForCourse(courseId);
				System.out.println("Which Evaluation would you like to delete? Please Enter EvaluationID");
				int evalId = in.nextInt();
				in.nextLine();
				if(isValidEvaluation(evalId, courseId) ) {
					String query = "DELETE FROM Evaluations WHERE EvaluationID=? AND CourseID=?";                        
					PreparedStatement ps = con.prepareStatement(query);
					ps.setInt(1, evalId);
					ps.setInt(2, courseId);
					ps.executeQuery();
					viewCourseByID(facultyId);
					System.err.printf("Course %d has been deleted\n", courseId);
					viewEvaluationForCourse(courseId);
				} else System.err.println("You have no such evaluation");
			} else System.err.println("You have no such course");
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}
	public void viewGrades(int courseId) {
		try {
			System.out.printf("List of all grades for course %d\n", courseId);
			String query = 
				"SELECT Evaluations.CourseID, Evaluations.EvaluationID, Evaluations.EvaluationType, Grades.StudentID, Students.StudentName, Grades.Grade FROM Grades, Evaluations, Students WHERE Evaluations.CourseID=? AND Grades.EvaluationID=Evaluations.EvaluationID AND Students.StudentID=Grades.StudentID";                        
			PreparedStatement ps = con.prepareStatement(query);
			ps.setInt(1, courseId);
			ResultSet rs = ps.executeQuery();
			ResultSetMetaData rsmd = rs.getMetaData();
			int columnsNumber = rsmd.getColumnCount();
			if(rs.next() == false)
				System.out.println("There are no grades for this course");
			else {
				System.out.println("CourseID, EvaluationID, EvaluationType, StudentID, StudentName, Grade");
				for(int i = 1; i <= columnsNumber; i++)
					System.out.print(rs.getString(i) + " \t|");
				System.out.println();

			}
			while ( rs.next()) {
				for(int i = 1; i <= columnsNumber; i++)
					System.out.print(rs.getString(i) + " \t|");
				System.out.println();
			}

		} catch (SQLException e) {
			e.printStackTrace();
		}
	}
	public void enterGrade( int facultyId) {
		try {
			viewCourseByID(facultyId);
			System.out.println("Select a course from above.");
			int courseId = in.nextInt();
			in.nextLine();
			if(isValidCourse(facultyId, courseId) ) {
				viewEvaluationForCourse(courseId);
				System.out.println("Select an Evauation from above.");
				int evalId = in.nextInt();
				in.nextLine();
				if (isValidEvaluation(evalId, courseId)) {
					viewStudentsInCourse(courseId);
					System.out.println("Select a Student from above.");
					int studentId = in.nextInt();
					in.nextLine();
					if(studentIsEnrolled(studentId, courseId)) {
						viewGrades(courseId);
						System.out.println("Please enter the Students grade.");
						double grade = in.nextDouble();
						in.nextLine();
						String query = "INSERT INTO Grades (Grade, StudentID, EvaluationID) VALUES (?, ?, ?)";
						PreparedStatement ps = con.prepareStatement(query);
						ps.setDouble(1, grade);
						ps.setInt(2, studentId);
						ps.setInt(3, evalId);
						ps.executeQuery();
						viewGrades(courseId);

					} else System.err.println("Student isn't enrolled");
				} else System.err.println("You have no such evaluation");
			} else System.err.println("You have no such course");
		} catch (SQLIntegrityConstraintViolationException e) {
			System.err.println("You have already graded this student");	
		}catch (SQLException e) {
			e.printStackTrace();
		}
	}

	public void printClassReport(int facultyId) {
		//courseName, meetsAt, roomNo, numStudents, numEvaluations
			System.out.println( "CourseName, meetsAt, roomNo, numStudents, numEvaluations");
		try {
			String query = "SELECT * FROM Courses WHERE facultyID=" +facultyId;
			PreparedStatement ps = con.prepareStatement(query);
			ResultSet rs = ps.executeQuery();
			int x, y;
			if( rs.next()== false)  {
			System.out.print("You have no courses");
			} else {
			query = "SELECT COUNT(StudentID) AS numStudents FROM Enrollment WHERE CourseID=" + rs.getInt("CourseID");
			ps = con.prepareStatement(query);
			ResultSet rs2 = ps.executeQuery();
			rs2.next();
			x = rs2.getInt("numStudents");
			query = "SELECT COUNT(EvaluationID) AS numEvals FROM Evaluations WHERE CourseID=" + rs.getInt("CourseID");
			ps = con.prepareStatement(query);
			rs2 = ps.executeQuery();
			rs2.next();
			y = rs2.getInt("numEvals");
			String r = "" +  rs.getString("CourseName") + "\t|" + rs.getString("MeetAt") + "\t|" + rs.getString("Room") + "\t|" + x + "\t|" + y + "\t|\n";
			System.out.println(r);
			while ( rs.next()) {
				query = "SELECT COUNT(StudentID) AS numStudents FROM Enrollment WHERE CourseID=" + rs.getInt("CourseID");
				ps = con.prepareStatement(query);
				rs2 = ps.executeQuery();
				rs2.next();
				x = rs2.getInt("numStudents");
				query = "SELECT COUNT(EvaluationID) AS numEvals FROM Evaluations WHERE CourseID=" + rs.getInt("CourseID");
				ps = con.prepareStatement(query);
				rs2 = ps.executeQuery();
				rs2.next();
				y = rs2.getInt("numEvals");
				r +=  rs.getInt("CourseName") + "\t|" + rs.getString("MeetAt") + "\t|" + rs.getString("Room") + "\t|" + x + "\t|" + y +"\n";
			}
			}
		}catch (SQLException e) {
			e.printStackTrace();
		}
	}



	public void printStudentGrade(int facultyId) {
		//courseName, semester, year, studentName, currentGrade
		System.out.println( "CourseName, Semester, Year, studentName, currentGrade");
		try {
			String query = "SELECT * FROM Courses WHERE facultyID=" +facultyId;
			PreparedStatement ps = con.prepareStatement(query);
			ResultSet rs = ps.executeQuery();
			if( rs.next()== false)  {
				System.out.print("You have no courses");
			} else {
				query = "SELECT Courses.CourseName, Courses.Semester, Courses.Year, Students.StudentName, Grades.Grade  FROM Grades, Students, Courses WHERE Grades.StudentID=Students.StudentID AND Courses.CourseID=" + rs.getInt("CourseID");
				ps = con.prepareStatement(query);
				ResultSet rs2 = ps.executeQuery();
				ResultSetMetaData rsmd = rs2.getMetaData();
				int columnsNumber = rsmd.getColumnCount();
				if( rs2.next() != false) {
					for(int i = 1; i <= columnsNumber; i++)
						System.out.print(rs.getString(i) + " \t|");
					System.out.println();
				}
				while ( rs2.next()) {
					for(int i = 1; i <= columnsNumber; i++)
						System.out.print(rs.getString(i) + " \t|");
					System.out.println();
				}
			}
			while ( rs.next()) {
				query = "SELECT Courses.CourseName, Courses.Semester, Courses.Year, Students.StudentName, Grades.Grade  FROM Grades, Students, Courses WHERE Grades.StudentID=Students.StudentID AND Courses.CourseID=" + rs.getInt("CourseID");
				ps = con.prepareStatement(query);
				ResultSet rs2 = ps.executeQuery();
				ResultSetMetaData rsmd = rs2.getMetaData();
				int columnsNumber = rsmd.getColumnCount();
				if( rs2.next() != false) {
					for(int i = 1; i <= columnsNumber; i++)
						System.out.print(rs.getString(i) + " \t|");
					System.out.println();
				}
				while ( rs2.next()) {
					for(int i = 1; i <= columnsNumber; i++)
						System.out.print(rs.getString(i) + " \t|");
					System.out.println();
				}
			}
		}
		catch (SQLException e) {
			e.printStackTrace();
		}
	}
	public void viewEvaluations(int studentId) {
		try {
			System.out.printf("Evaluations for student %d LIST:\n", studentId);
			String query = "SELECT Evaluations.CourseID, Evaluations.EvaluationType, Evaluations.EvaluationWeight, Evaluations.EvaluationDeadline, Evaluations.EvaluationMeetingRoom FROM Enrollment, Evaluations WHERE Enrollment.CourseID=Evaluations.CourseID AND Enrollment.StudentID=?";                        
			PreparedStatement ps = con.prepareStatement(query);
			ps.setInt(1, studentId);
			ResultSet rs = ps.executeQuery();
			ResultSetMetaData rsmd = rs.getMetaData();
			int columnsNumber = rsmd.getColumnCount();
			if(rs.next() == false)
				System.out.println("There are no Evaluations for this course");
			else {
				System.out.println("CourseID, EvaluationType, EvaluationWeight, EvaluationDeadline, EvaluationMeetingRoom ");
				for(int i = 1; i <= columnsNumber; i++)
					System.out.print(rs.getString(i) + " \t|");
				System.out.println();

			}
			while ( rs.next()) {
				for(int i = 1; i <= columnsNumber; i++)
					System.out.print(rs.getString(i) + " \t|");
				System.out.println();
			}
		}catch (SQLException e) {
			e.printStackTrace();
		}

	}
	public void viewCourses(int studentId) {
		try {
			System.out.printf("Course for student %d LIST:\n", studentId);
			String query = "SELECT Enrollment.CourseID, Courses.CourseName, Courses.Semester, Courses.Year, Courses.MeetAt, Courses.Room  FROM Enrollment, Courses WHERE Enrollment.CourseID=Courses.CourseID AND Enrollment.StudentID=?";                       
			PreparedStatement ps = con.prepareStatement(query);
			ps.setInt(1, studentId);
			ResultSet rs = ps.executeQuery();
			ResultSetMetaData rsmd = rs.getMetaData();
			int columnsNumber = rsmd.getColumnCount();
			if(rs.next() == false)
				System.out.println("There are no Courses");
			else {
				System.out.println("CourseID, CourseName, Semester, Year, MeetAt, Room");
				for(int i = 1; i <= columnsNumber; i++)
					System.out.print(rs.getString(i) + " \t|");
				System.out.println();

			}
			while ( rs.next()) {
				for(int i = 1; i <= columnsNumber; i++)
					System.out.print(rs.getString(i) + " \t|");
				System.out.println();
			}
		}catch (SQLException e) {
			e.printStackTrace();
		}

	}
	public void viewallGrades(int studentId) {
		try {
			System.out.printf("Course for student %d LIST:\n", studentId);
			String query = "SELECT Grades.Grade, Courses.CourseName, Evaluations.EvaluationType FROM GRADES, Evaluations, Courses WHERE Grades.EvaluationID=Evaluations.EvaluationID AND Courses.CourseID=Evaluations.CourseID AND Grades.StudentID=?";                       
			PreparedStatement ps = con.prepareStatement(query);
			ps.setInt(1, studentId);
			ResultSet rs = ps.executeQuery();
			ResultSetMetaData rsmd = rs.getMetaData();
			int columnsNumber = rsmd.getColumnCount();
			if(rs.next() == false)
				System.out.println("There are no Grades");
			else {
				System.out.println("Grade, CourseName, EvaluationType");
				for(int i = 1; i <= columnsNumber; i++)
					System.out.print(rs.getString(i) + " \t|");
				System.out.println();

			}
			while ( rs.next()) {
				for(int i = 1; i <= columnsNumber; i++)
					System.out.print(rs.getString(i) + " \t|");
				System.out.println();
			}
		}catch (SQLException e) {
			e.printStackTrace();
		}

	}
	public void viewDepartments() {
		try {
			String query = "SELECT * FROM Departments";
			PreparedStatement ps = con.prepareStatement(query);
			ResultSet rs = ps.executeQuery();
			ResultSetMetaData rsmd = rs.getMetaData();
			int columnsNumber = rsmd.getColumnCount();
			if(rs.next() == false)
				System.out.println("There are no Departments");
			else {
				System.out.println("DepartmentID, DepartmantName, DepartmentHead ");
				for(int i = 1; i <= columnsNumber; i++)
					System.out.print(rs.getString(i) + " \t|");
				System.out.println();

			}
			while ( rs.next()) {
				for(int i = 1; i <= columnsNumber; i++)
					System.out.print(rs.getString(i) + " \t|");
				System.out.println();
			}

		}catch (SQLException e) {
			e.printStackTrace();
		}

	}
	public void viewFaculty() {
		try {
			String query = "SELECT * FROM Faculties";
			PreparedStatement ps = con.prepareStatement(query);
			ResultSet rs = ps.executeQuery();
			ResultSetMetaData rsmd = rs.getMetaData();
			int columnsNumber = rsmd.getColumnCount();
			if(rs.next() == false)
				System.out.println("There are no Faculty");
			else {
				System.out.println("FacultyID, FacultyName, DepartmentID ");
				for(int i = 1; i <= columnsNumber; i++)
					System.out.print(rs.getString(i) + " \t|");
				System.out.println();

			}
			while ( rs.next()) {
				for(int i = 1; i <= columnsNumber; i++)
					System.out.print(rs.getString(i) + " \t|");
				System.out.println();
			}

		}catch (SQLException e) {
			e.printStackTrace();
		}

	}

	public static void main( String [] args ) {
		CourseManagementDB cmdb = new CourseManagementDB();
		int insert = 0;
		while (insert != 4) {
			System.out.println( "Welcome to the University Course Management System");
			System.out.println( "1. To access Faculty");
			System.out.println( "2. To access Student");
			System.out.println( "3. To access Admin");
			System.out.println( "4. To Quit");
			insert = in.nextInt();
			in.nextLine();
			switch(insert) {
				case 1:
					int facultyId;
					String facultyName = "";
					System.out.println( "Welcome Faculty, Please enter your Faculty ID?");
					facultyId = in.nextInt();
					in.nextLine(); 
					String query = "select FacultyName from Faculties where FacultyID=?";
					try {
						PreparedStatement ps = cmdb.con.prepareStatement(query);
						ps.setInt(1, facultyId);
						ResultSet ans= ps.executeQuery();
						while ( ans.next() ) {
							facultyName = ans.getString("FacultyName");
							System.out.println(facultyName);
						}
						if(facultyName.equals("")) {
							System.err.println("This id doesnt exist");
							break;
						}
					}catch ( SQLException e) {
						System.out.println("Something went wrong");
						e.printStackTrace();
						break;
					}
					int instruction = 0;
					while (instruction != 7) {
						System.out.printf( "Welcome %s, What would you like to do?\n", facultyName);
						System.out.println( "1. To Create/Update/View/Delete a course");
						System.out.println( "2. To Enroll students in a course");
						System.out.println( "3. To Create/Update/View/Delete an evaluation");
						System.out.println( "4. To Enter Grades");
						System.out.println( "5. To see Reports of Classes");
						System.out.println( "6. To see Reports of Students and Grades");
						System.out.println( "7. To Quit");
						instruction = in.nextInt();
						in.nextLine();
						switch (instruction) {
							case 1: //
								int addUpdate = 0;
								while ( true) {
									System.out.println("Do you want to add/update/view your courses?\n1. To ADD\n2. To UPDATE\n3. To VIEW\n4. To DELETE\n5. To QUIT");
									addUpdate = in.nextInt();
									in.nextLine();
									if (addUpdate == 1) 
										cmdb.addNewCourse(facultyId);
									else if (addUpdate == 2) 
										cmdb.updateCourse(facultyId);
									else if (addUpdate == 3)
										cmdb.viewCourseByID(facultyId);
									else if (addUpdate == 4)
										cmdb.deleteCourse(facultyId);
									else if (addUpdate == 5)
										break;
									else 
										System.out.println("Wrong Input");
								}
								break;
							case 2:
								cmdb.assignStudent(facultyId);
								break;
							case 3:
								int addEvaluation = 0;
								while ( true) {
									System.out.println("Do you want to add/update/view your evaluation?\n1. To ADD\n2. To UPDATE\n3. To VIEW\n4. To DELETE\n5. To QUIT");
									addEvaluation = in.nextInt();
									in.nextLine();
									if (addEvaluation == 1) 
										cmdb.addNewEvaluation(facultyId);
									else if (addEvaluation == 2) 
										cmdb.updateEvaluation(facultyId);
									else if (addEvaluation == 3)
										cmdb.viewEvaluation(facultyId);
									else if (addEvaluation == 4)
										cmdb.deleteEvaluation(facultyId);
									else if (addEvaluation == 5)
										break;
									else 
										System.out.println("Wrong Input");
								}
								break;
							case 4:
								cmdb.enterGrade(facultyId);
								break;
							case 5:
								cmdb.printClassReport(facultyId);
								break;
							case 6:
								cmdb.printStudentGrade(facultyId);
								break;
							case 7:
								break;
							default :
								System.out.println("Wrong Input");
								break;
						}
					}
					break;
				case 2:
					int studentId;
					String studentName = "";
					System.out.println( "Welcome Student, Please enter your Student ID?");
					studentId = in.nextInt();
					in.nextLine(); 
					query = "SELECT StudentName FROM Students where StudentID=?";
					try {
						PreparedStatement ps = cmdb.con.prepareStatement(query);
						ps.setInt(1, studentId);
						ResultSet ans= ps.executeQuery();
						while ( ans.next() ) {
							studentName = ans.getString("StudentName");
							System.out.println(studentName);
						}
						if(studentName.equals("")) {
							System.err.println("This id doesnt exist");
							break;
						}
					}catch ( SQLException e) {
						System.out.println("Something went wrong");
						e.printStackTrace();
						break;
					}
					instruction = 0;
					while (instruction != 4) {
						System.out.println( "Welcome Student, What would you like to do?");
						System.out.println( "1. To see Calendar of evaluations");
						System.out.println( "2. To see your Courses");
						System.out.println( "3. To see your Grade");
						System.out.println( "4. To Quit");
						instruction = in.nextInt();
						in.nextLine();
						switch( instruction) {
							case 1:
								cmdb.viewEvaluations(studentId);
								break;
							case 2:
								cmdb.viewCourses(studentId);
								break;
							case 3:
								cmdb.viewallGrades(studentId);
								break;
							case 4:
								break;
							default:
								System.out.println("Wrong Input");
								break;
						}
					}
					break;
				case 3:
					System.out.println( "Welcome Admin");
					instruction = 0;
					while (instruction != 3) {
						System.out.println( "Welcome Admin, What would you like to do?");
						System.out.println( "1. To see Department Report");
						System.out.println( "2. To see Faculty Report");	
						System.out.println( "3. To Quit");
						instruction = in.nextInt();
						in.nextLine();
						switch(instruction) {
							case 1:
								cmdb.viewDepartments();
								break;
							case 2:
								cmdb.viewFaculty();
								break;
							case 3:
							break;
							default:
								System.out.println("Wrong Input");
								break;
						}
					}
					break;
				default:
					break;

			}


		}

		//sdb.getStudentsInDepartment(11);
		//System.out.println( "-----------------" );
		//sdb.getStudentsInDepartmentPrepared(11);
		//System.out.println( "-----------------" );
		//sdb.enroll( 4181, "ENG40000","A" );
		//System.out.println( "-----------------" );
		//sdb.insertNewDepartment();
	}
}


