use std::fs;
use std::io::{self, Write};
use std::path::Path;

fn main() {
    loop {
        println!("\n===== 파일 및 디렉토리 실습 메뉴 =====");
        println!("1. 디렉토리 생성");
        println!("2. 디렉토리 목록 출력");
        println!("3. 파일 생성");
        println!("4. 파일에 내용 쓰기");
        println!("5. 파일 읽기");
        println!("6. 파일 삭제");
        println!("7. 종료");

        print!("메뉴 선택: ");
        io::stdout().flush().unwrap();

        let mut choice = String::new();
        io::stdin().read_line(&mut choice).unwrap();
        let choice = choice.trim();

        match choice {
            "1" => create_directory(),
            "2" => list_directory(),
            "3" => create_file(),
            "4" => write_file(),
            "5" => read_file(),
            "6" => delete_file(),
            "7" => {
                println!("프로그램 종료");
                break;
            }
            _ => println!("잘못된 입력입니다. 다시 선택하세요."),
        }
    }
}

fn input(prompt: &str) -> String {
    print!("{}", prompt);
    io::stdout().flush().unwrap();

    let mut text = String::new();
    io::stdin().read_line(&mut text).unwrap();
    text.trim().to_string()
}

// 1. 디렉토리 생성
fn create_directory() {
    let dirname = input("생성할 디렉토리 이름: ");
    match fs::create_dir(&dirname) {
        Ok(_) => println!("디렉토리 '{}' 생성 완료!", dirname),
        Err(e) => println!("오류 발생: {}", e),
    }
}

// 2. 디렉토리 목록 출력
fn list_directory() {
    let dirname = input("목록을 볼 디렉토리 이름: ");

    match fs::read_dir(&dirname) {
        Ok(entries) => {
            println!("'{}' 디렉토리 목록:", dirname);
            for entry in entries {
                if let Ok(entry) = entry {
                    println!("- {}", entry.file_name().to_string_lossy());
                }
            }
        }
        Err(e) => println!("오류: {}", e),
    }
}

// 3. 파일 생성
fn create_file() {
    let filename = input("생성할 파일 이름: ");

    match fs::File::create(&filename) {
        Ok(_) => println!("파일 '{}' 생성 완료!", filename),
        Err(e) => println!("오류: {}", e),
    }
}

// 4. 파일에 내용 쓰기
fn write_file() {
    let filename = input("쓰기할 파일 이름: ");

    if !Path::new(&filename).exists() {
        println!("파일이 존재하지 않습니다.");
        return;
    }

    let text = input("파일에 쓸 내용: ");

    match fs::write(&filename, text) {
        Ok(_) => println!("쓰기 완료!"),
        Err(e) => println!("오류: {}", e),
    }
}

// 5. 파일 읽기
fn read_file() {
    let filename = input("읽을 파일 이름: ");

    match fs::read_to_string(&filename) {
        Ok(content) => {
            println!("===== 파일 내용 =====");
            println!("{}", content);
        }
        Err(e) => println!("오류: {}", e),
    }
}

// 6. 파일 삭제
fn delete_file() {
    let filename = input("삭제할 파일 이름: ");

    match fs::remove_file(&filename) {
        Ok(_) => println!("파일 '{}' 삭제 완료!", filename),
        Err(e) => println!("오류: {}", e),
    }
}
