// build.rs

// Bring in a dependency on an externally maintained `gcc` package which manages
// invoking the C compiler.
extern crate gcc;

fn main() {
    gcc::Config::new()
                .file("minimal.cpp")
                .cpp(true)
                .include("src")
                .compile("libminimal.a");    
/*    let conf = gcc::new();
    conf.cpp(true);
    gcc::compile_library("libminimal.a", &["minimal.cpp"]);
*/
}


