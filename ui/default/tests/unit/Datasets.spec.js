import { describe, expect, test } from 'vitest'
import { helpers } from '../../src/modules/dataStore.js';

describe('Gets correct number of datasets', () => {

    test('Should return 0 datasets', () => {
        let data = [
            
        ]
        
        let result = helpers.GetDataSets(data);

        expect(result.length).toEqual(0);
    
    })

    test('Should return 2 datasets', () => {
        let data = [
            {id: 0, t: 0, set: 0, theta: 0, omega: 0},
            {id: 1, t: 1, set: 0, theta: 0, omega: 0},
            {id: 2, t: 2, set: 0, theta: 0, omega: 0},
            {id: 3, t: 3, set: 0, theta: 0, omega: 0},
            {id: 4, t: 4, set: 0, theta: 0, omega: 0},
            {id: 5, t: 5, set: 1, theta: 0, omega: 0},
            {id: 6, t: 6, set: 1, theta: 0, omega: 0},
            {id: 7, t: 7, set: 1, theta: 0, omega: 0},
            {id: 8, t: 8, set: 1, theta: 0, omega: 0},
            {id: 9, t: 9, set: 1, theta: 0, omega: 0},
        ]
        
        let result = helpers.GetDataSets(data);

        expect(result.length).toEqual(2);
    
    })

    test('Should return 3 datasets', () => {
        let data = [
            {id: 0, t: 0, set: 0, theta: 0, omega: 0},
            {id: 1, t: 1, set: 0, theta: 0, omega: 0},
            {id: 2, t: 2, set: 0, theta: 0, omega: 0},
            {id: 3, t: 3, set: 0, theta: 0, omega: 0},
            {id: 4, t: 4, set: 1, theta: 0, omega: 0},
            {id: 5, t: 5, set: 1, theta: 0, omega: 0},
            {id: 6, t: 6, set: 1, theta: 0, omega: 0},
            {id: 7, t: 7, set: 2, theta: 0, omega: 0},
            {id: 8, t: 8, set: 2, theta: 0, omega: 0},
            {id: 9, t: 9, set: 2, theta: 0, omega: 0},
        ]
        
        let result = helpers.GetDataSets(data);

        expect(result.length).toEqual(3);
    
    })

    test('Should return 1 datasets', () => {
        let data = [
            {id: 0, t: 0, set: 0, theta: 0, omega: 0},
            {id: 1, t: 1, set: 0, theta: 0, omega: 0},
            {id: 2, t: 2, set: 0, theta: 0, omega: 0},
            {id: 3, t: 3, set: 0, theta: 0, omega: 0},
            {id: 4, t: 4, set: 0, theta: 0, omega: 0},
            {id: 5, t: 5, set: 0, theta: 0, omega: 0},
            {id: 6, t: 6, set: 0, theta: 0, omega: 0},
            {id: 7, t: 7, set: 0, theta: 0, omega: 0},
            {id: 8, t: 8, set: 0, theta: 0, omega: 0},
            {id: 9, t: 9, set: 0, theta: 0, omega: 0},
        ]
        
        let result = helpers.GetDataSets(data);

        expect(result.length).toEqual(1);
    
    })

    

})


describe('Returns correct datasets', () => {

    test('Should return empty array', () => {
        let data = [
            
        ]
        
        let result = helpers.GetDataSets(data);

        expect(result).toEqual([])
    })

    test('Should return 1 array in an array', () => {
        let data = [
            {id: 0, t: 0, set: 0, theta: 0, omega: 0},
            {id: 1, t: 1, set: 0, theta: 0, omega: 0},
            {id: 2, t: 2, set: 0, theta: 0, omega: 0},
            {id: 3, t: 3, set: 0, theta: 0, omega: 0}
        ]
        
        let result = helpers.GetDataSets(data);

        expect(result).toEqual([[{id: 0, t: 0, set: 0, theta: 0, omega: 0},
            {id: 1, t: 1, set: 0, theta: 0, omega: 0},
            {id: 2, t: 2, set: 0, theta: 0, omega: 0},
            {id: 3, t: 3, set: 0, theta: 0, omega: 0}]])
    })

    test('Should return 2 arrays in an array', () => {
        let data = [
            {id: 0, t: 0, set: 0, theta: 0, omega: 0},
            {id: 1, t: 1, set: 0, theta: 0, omega: 0},
            {id: 2, t: 2, set: 1, theta: 0, omega: 0},
            {id: 3, t: 3, set: 1, theta: 0, omega: 0}
        ]
        
        let result = helpers.GetDataSets(data);

        expect(result).toEqual([
            [{id: 0, t: 0, set: 0, theta: 0, omega: 0},
            {id: 1, t: 1, set: 0, theta: 0, omega: 0}],
            [{id: 2, t: 2, set: 1, theta: 0, omega: 0},
            {id: 3, t: 3, set: 1, theta: 0, omega: 0}]
        ])
    })

    

   

   

    

})