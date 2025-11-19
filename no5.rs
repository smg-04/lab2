use std::fs;
use std::io;
use std::path::Path;

fn list_recursive(path: &Path) -> io::Result<()> {
    // 디렉토리 경로 출력
    println!("\n{}:", path.display());

    // 현재 디렉토리의 항목들 읽기
    let entries = fs::read_dir(path)?;

    // 먼저 파일 및 디렉토리 이름 출력
    let mut subdirs = Vec::new();

    for entry in entries {
        let entry = entry?;
        let file_name = entry.file_name();
        let file_name_str = file_name.to_string_lossy().to_string();

        let file_path = entry.path();

        // 파일/디렉토리 이름 출력
        println!("{}", file_name_str);

        // 디렉토리라면 재귀 처리를 위해 목록에 저장
        if file_path.is_dir() {
            subdirs.push(file_path);
        }
    }

    // 디렉토리들을 재귀적으로 방문
    for subdir in subdirs {
        list_recursive(&subdir)?;
    }

    Ok(())
}

fn main() {
    // 사용자에게 시작할 디렉토리 입력받기
    println!("재귀적으로 탐색할 디렉토리 경로를 입력하세요:");
    
    let mut dir = String::new();
    std::io::stdin().read_line(&mut dir).unwrap();
    let dir = dir.trim();

    let path = Path::new(dir);

    if !path.exists() {
        println!("경로가 존재하지 않습니다.");
        return;
    }

    if let Err(e) = list_recursive(path) {
        println!("오류 발생: {}", e);
    }
}
