use std::io::{self, Write};

fn read_usize(prompt: &str) -> usize {
    print!("{}", prompt);
    io::stdout().flush().unwrap();

    let mut input = String::new();
    io::stdin().read_line(&mut input).unwrap();
    input.trim().parse::<usize>().expect("정수를 입력하세요.")
}

fn read_matrix(rows: usize, cols: usize, name: &str) -> Vec<Vec<i32>> {
    println!("행렬 {} 입력:", name);

    let mut matrix = Vec::with_capacity(rows);

    for r in 0..rows {
        println!("{}행 입력 ({}개의 정수):", r + 1, cols);

        let mut input = String::new();
        io::stdin().read_line(&mut input).unwrap();

        let row: Vec<i32> = input
            .split_whitespace()
            .take(cols)
            .map(|x| x.parse::<i32>().expect("정수를 입력하세요."))
            .collect();

        if row.len() != cols {
            panic!("정확히 {}개의 값을 입력해야 합니다.", cols);
        }

        matrix.push(row);
    }

    matrix
}

fn add_matrices(a: &Vec<Vec<i32>>, b: &Vec<Vec<i32>>) -> Vec<Vec<i32>> {
    let rows = a.len();
    let cols = a[0].len();

    let mut result = Vec::with_capacity(rows);

    for i in 0..rows {
        let mut row = Vec::with_capacity(cols);
        for j in 0..cols {
            row.push(a[i][j] + b[i][j]);
        }
        result.push(row);
    }

    result
}

fn print_matrix(matrix: &Vec<Vec<i32>>, name: &str) {
    println!("행렬 {}:", name);
    for row in matrix {
        for val in row {
            print!("{:4} ", val);
        }
        println!();
    }
}

fn main() {
    // 1. 크기 입력
    let rows = read_usize("행 개수를 입력하세요: ");
    let cols = read_usize("열 개수를 입력하세요: ");

    // 2. 두 행렬 입력
    let matrix_a = read_matrix(rows, cols, "A");
    let matrix_b = read_matrix(rows, cols, "B");

    // 3. 행렬 덧셈
    let sum = add_matrices(&matrix_a, &matrix_b);

    // 4. 출력
    print_matrix(&matrix_a, "A");
    print_matrix(&matrix_b, "B");
    print_matrix(&sum, "A + B");
}