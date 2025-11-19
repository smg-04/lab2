use std::io::{self, Write};
use std::time::Instant;
use std::path::Path;

fn prompt(s: &str) -> String {
    print!("{}", s);
    io::stdout().flush().unwrap();
    let mut buf = String::new();
    io::stdin().read_line(&mut buf).unwrap();
    // 줄바꿈(\n,\r\n)만 제거 — 사용자가 입력한 앞/뒤의 공백은 유지
    buf.trim_end_matches(&['\r', '\n'][..]).to_string()
}

/// 두 문자열을 문자단위로 비교하여 (맞은 문자 수, 틀린(오타) 문자 수)를 반환
fn compare_chars(target: &str, typed: &str) -> (usize, usize) {
    let t_chars: Vec<char> = target.chars().collect();
    let p_chars: Vec<char> = typed.chars().collect();

    let min_len = t_chars.len().min(p_chars.len());
    let mut correct = 0usize;

    for i in 0..min_len {
        if t_chars[i] == p_chars[i] {
            correct += 1;
        }
    }

    // 같은 길이 부분에서의 불일치 + 길이 차이(삽입/삭제)
    let mismatches = (min_len - correct) + (t_chars.len().max(p_chars.len()) - min_len);

    (correct, mismatches)
}

fn main() {
    // 내장 문장 목록 (원하면 파일에서 읽도록 확장 가능)
    let sentences = vec![
        "The quick brown fox jumps over the lazy dog.",
        "Rust is a systems programming language focused on safety.",
        "Hello, world!",
        "Typing practice improves speed and accuracy.",
        "Keep calm and code in Rust.",
        "나는 오늘 러스트로 타자 연습을 한다.",
        "프로그래밍은 문제 해결의 연속이다.",
        "연습은 완벽을 만든다.",
        "파일과 디렉토리를 다루는 연습도 중요하다.",
        "테스트 케이스를 많이 만들어 보자.",
    ];

    println!("== 타자 연습 프로그램 ==");
    println!("문장 목록은 프로그램 내부의 샘플을 사용합니다.");
    println!("입력 시 문장의 공백(스페이스)도 포함하여 정확히 타이핑하세요.\n");

    // 시작할 문장 수 입력
    let n_input = prompt("몇 개의 문장을 연습하시겠습니까? (숫자 입력): ");
    let n: usize = match n_input.trim().parse() {
        Ok(x) if x > 0 => x,
        _ => {
            println!("올바른 양의 정수를 입력하세요.");
            return;
        }
    };

    // 시작 디렉토리(옵션) — 단순히 현재 작업 디렉토리 표시(설명 목적)
    if let Ok(cwd) = std::env::current_dir() {
        println!("(현재 작업 디렉토리: {})", cwd.display());
    }

    println!("\n준비되면 Enter를 눌러 연습을 시작하세요.");
    let _ = prompt("");

    let mut total_correct_chars: usize = 0;
    let mut total_mistakes: usize = 0;
    let mut total_time_secs: f64 = 0.0;
    let mut per_sentence_wpm: Vec<f64> = Vec::new();

    for i in 0..n {
        // 문장 선택: 목록을 순환해서 사용
        let idx = i % sentences.len();
        let target = sentences[idx];

        println!("\n문장 {}/{}:", i + 1, n);
        println!("{}", target);
        println!("타이핑을 시작하면 Enter를 누르세요 (타이머 시작)...");
        let _ = prompt("");

        let start = Instant::now();
        let typed = prompt("> "); // 사용자가 입력한 문자열(공백 유지)
        let duration = start.elapsed();
        let duration_secs = duration.as_secs_f64();

        let (correct, mistakes) = compare_chars(target, &typed);
        let wpm = if duration_secs > 0.0 {
            // WPM 계산: (올바르게 친 문자 수 / 5) / (minutes)
            let minutes = duration_secs / 60.0;
            (correct as f64 / 5.0) / minutes
        } else {
            0.0
        };

        println!("소요시간: {:.2}초", duration_secs);
        println!("맞은 문자: {}   오타(틀린 문자): {}", correct, mistakes);
        println!("이번 문장 WPM: {:.2}", wpm);

        total_correct_chars += correct;
        total_mistakes += mistakes;
        total_time_secs += duration_secs;
        per_sentence_wpm.push(wpm);
    }

    // 전체 WPM: 전체 정답 문자 기준
    let overall_wpm = if total_time_secs > 0.0 {
        let minutes = total_time_secs / 60.0;
        (total_correct_chars as f64 / 5.0) / minutes
    } else {
        0.0
    };

    // 평균 WPM (문장별 평균) — 선택적으로 표시
    let mean_wpm_per_sentence = if !per_sentence_wpm.is_empty() {
        let sum: f64 = per_sentence_wpm.iter().sum();
        sum / per_sentence_wpm.len() as f64
    } else {
        0.0
    };

    println!("\n===== 연습 결과 요약 =====");
    println!("총 연습 문장 수: {}", n);
    println!("총 맞은 문자 수: {}", total_correct_chars);
    println!("총 오타(틀린 문자) 수: {}", total_mistakes);
    println!("총 소요시간: {:.2}초", total_time_secs);
    println!("전체 기준 WPM (전체 올바른 문자 사용): {:.2}", overall_wpm);
    println!("문장별 평균 WPM: {:.2}", mean_wpm_per_sentence);
    println!("(참고: WPM 계산은 '문자 5개 = 1단어' 규칙을 따릅니다.)");

    println!("\n프로그램 종료. 더 연습하고 싶으면 다시 실행하세요!");
}
