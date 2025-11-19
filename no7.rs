use std::io::{self, Write};
use std::process::{Command, Stdio};
use signal_hook::consts::signal::*;
use signal_hook::iterator::Signals;
use std::thread;
use std::time::Duration;

fn main() -> std::io::Result<()> {
    // ==== 시그널 핸들러 설정 ====
    let mut signals = Signals::new(&[SIGINT, SIGTERM])?;

    // 시그널 처리 스레드
    thread::spawn(move || {
        for sig in signals.forever() {
            match sig {
                SIGINT => println!("\n[시그널] SIGINT(Ctrl + C) 감지!"),
                SIGTERM => println!("\n[시그널] SIGTERM 감지!"),
                _ => println!("\n[시그널] 기타 시그널 감지: {}", sig),
            }
        }
    });

    loop {
        println!("\n====== 프로세스 & 시그널 실습 ======");
        println!("1. 새로운 프로세스 실행 (ls)");
        println!("2. 새로운 프로세스 실행 (sleep 5)");
        println!("3. 무한루프 시그널 테스트");
        println!("4. 종료");

        print!("메뉴 선택: ");
        io::stdout().flush().unwrap();

        let mut menu = String::new();
        io::stdin().read_line(&mut menu).unwrap();

        match menu.trim() {
            "1" => run_ls(),
            "2" => run_sleep(),
            "3" => infinite_loop_test(),
            "4" => {
                println!("프로그램 종료");
                break;
            }
            _ => println!("잘못된 선택입니다."),
        }
    }

    Ok(())
}

// =========================================
// 1. ls 명령 실행
// =========================================
fn run_ls() {
    println!("\n[실행] ls 명령 실행\n");

    let output = Command::new("ls")
        .arg("-l")
        .stdout(Stdio::piped())
        .output()
        .expect("ls 실행 실패");

    println!("{}", String::from_utf8_lossy(&output.stdout));
}

// =========================================
// 2. sleep 5 실행
// =========================================
fn run_sleep() {
    println!("\n[실행] sleep 5 실행 (5초 기다리기)");

    let status = Command::new("sleep")
        .arg("5")
        .status()
        .expect("sleep 실행 실패");

    println!("[프로세스 종료] 상태 코드: {:?}", status.code());
}

// =========================================
// 3. 무한 루프 + SIGINT 테스트
// =========================================
fn infinite_loop_test() {
    println!("\n[시작] Ctrl+C를 눌러 SIGINT 테스트 가능!\n");

    for i in 1.. {
        println!("작동 중... {} (Ctrl+C 눌러 보세요)", i);
        thread::sleep(Duration::from_secs(1));
    }
}
