struct User {
	name string 
} 

struct Record {
	m map[string]int 
	users map[string]User 
}

fn (self mut Record) set(key string, val int) {
	self.m[key] = val
}

fn main() {
	mut m := map[string]int{}
	m['hi'] = 80
	assert m['hi'] == 80
	//// 
	mut users := map[string]User{} 
	users['1'] = User{'Peter'} 
	peter := users['1']
	assert  peter.name == 'Peter' 

	println("name = $peter.name")
	mut a := Record{
		m: map[string]int{} 
		users: map[string]User{} 
	}
	a.users['Bob'] = User{'Bob'} 
	q := a.users['Bob'] 
	assert q.name == 'Bob' 
	a.m['one'] = 1
	a.set('two', 2)
	assert a.m['one'] == 1
	assert a.m['two'] == 2
}
