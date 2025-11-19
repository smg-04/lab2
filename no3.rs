use std::collections::HashMap;
use std::io::{self, Write};

fn read_input(prompt: &str) -> String {
    print!("{}", prompt);
    io::stdout().flush().unwrap();

    let mut input = String::new();
    io::stdin().read_line(&mut input).unwrap();
    input.trim().to_string()
}

fn main() {
    let mut phonebook: HashMap<String, String> = HashMap::new();

    loop {
        println!("\n===== 전화번호부 메뉴 =====");
        println!("1. 추가");
        println!("2. 검색");
        println!("3. 종료");

        let choice = read_input("메뉴 선택: ");

        match choice.as_str() {
            "1" => {
                let name = read_input("이름 입력: ");
                let number = read_input("전화번호 입력: ");

                phonebook.insert(name.clone(), number.clone());
                println!("저장 완료! → {} : {}", name, number);
            }

            "2" => {
                let name = read_input("검색할 이름 입력: ");

                match phonebook.get(&name) {
                    Some(number) => println!("검색 결과 → {} : {}", name, number),
                    None => println!("'{}' 은(는) 전화번호부에 없습니다.", name),
                }
            }

            "3" => {
                println!("프로그램을 종료합니다.");
                break;
            }

            _ => {
                println!("잘못된 입력입니다. 1~3을 선택하세요.");
            }
        }
    }
}
